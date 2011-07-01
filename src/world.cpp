// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./world.h"

#include <ClanLib/core.h>
#include <list>

#include "./tankvehicle.h"
#include "./building.h"
#include "./background.h"
#include "./fly.h"

World::World (const CL_DisplayWindow &display_window)
  : window (display_window),
    quit (false),
    center_x (0),
    center_y (0),
    moving_down (false),
    moving_up (false),
    moving_left (false),
    moving_right (false) {
  CL_Slot slot_quit = window.sig_window_close().connect (this, &World::on_window_close);

  gc = window.get_gc();
  window_width = gc.get_width();
  window_height = gc.get_height();

  // Setup resources
  resources = CL_ResourceManager ("resources.xml");
  background = new CL_Sprite (gc, "Background", &resources);
  cross = new CL_Sprite (gc, "Cross", &resources);
  cross_x = window_width/2;
  cross_y = window_height/2;
  center_x = 300;
  center_y = 300;
  cross_speed = 0.7;

  width = background->get_width();
  height = background->get_height();

  // Receive mouse clicks
  slotKeyDown = window.get_ic().get_keyboard().sig_key_down().
                connect (this, &World::onKeyDown);
  slotKeyUp = window.get_ic().get_keyboard().sig_key_up().
              connect (this, &World::onKeyUp);
  slotMouseDown = window.get_ic().get_mouse().sig_key_down().
                  connect (this, &World::onMouseDown);
  slotMouseUp = window.get_ic().get_mouse().sig_key_up().
                connect (this, &World::onMouseUp);
  slotMouseMove = window.get_ic().get_mouse().sig_pointer_move().
                  connect (this, &World::onMouseMove);

  dragging = mouseDown = false;

  // Add some tanks
  initLevel();

  // Run the main loop
  run();
}

World::~World() {
  // Delete all gameobjects
  std::list<GameObject *>::iterator it;
  for (it = objects.begin(); it != objects.end(); ++it)
    delete (*it);

}

void World::initLevel() {
  Fly *fly = new Fly(Fly::SPACE_SHOOT, this);
  fly->setPos(400, 500);

  TankVehicle *tank1 = new TankVehicle (TankVehicle::SPACE_SHOOT, this);
  tank1->setPos (667, 207);

  TankVehicle *tank2 = new TankVehicle (TankVehicle::SPACE_SHOOT, this);
  tank2->setPos (854, 422);

  TankVehicle *tank3 = new TankVehicle (TankVehicle::SPACE_SHOOT, this);
  tank3->setPos (400, 422);

  Building *helipad = new Building (Building::HELI_PAD, this);
  helipad->setPos (254, 222);

  addObject(helipad);
  addTank(tank1);
  addTank(tank2);
  addTank(tank3);
  addObject(fly);
}

void World::addObject (GameObject *object) {
  objects.push_back (object);
}

void World::addTank (TankVehicle *tank) {
  objects.push_back (tank);
  tanks.push_back (tank);
}

bool World::hitCheck (CL_CollisionOutline *outline, GameObject *other) {
  std::list<GameObject *>::iterator it;
  for (it = objects.begin(); it != objects.end(); ++it) {
    if ( (*it) != other) {
      if ( (*it)->hitCheck (outline))
        return true;
    }
  }

  return false;
}

void World::onKeyDown (const CL_InputEvent &key, const CL_InputState &state) {
  // Fire missile on space
  if (key.id == CL_KEY_SPACE) {
    std::list<TankVehicle *>::iterator it;
    for (it = tanks.begin(); it != tanks.end(); ++it)
      (*it)->fire (false);
  }

  if (key.id == CL_KEY_DOWN) {
    moving_down = true;
  }

  if (key.id == CL_KEY_UP) {
    moving_up = true;
  }

  if (key.id == CL_KEY_LEFT) {
    moving_left = true;
  }

  if (key.id == CL_KEY_RIGHT) {
    moving_right = true;
  }

}

void World::onKeyUp (const CL_InputEvent &key, const CL_InputState &state) {


  if (key.id == CL_KEY_DOWN) {
    moving_down = false;
  }

  if (key.id == CL_KEY_UP) {
    moving_up = false;
  }

  if (key.id == CL_KEY_LEFT) {
    moving_left = false;
  }

  if (key.id == CL_KEY_RIGHT) {
    moving_right = false;
  }

}


void World::onMouseDown (const CL_InputEvent &key, const CL_InputState &state) {
  // Start dragging on left click
  if (key.id == CL_MOUSE_LEFT) {
    dragArea.left = key.mouse_pos.x;
    dragArea.top = key.mouse_pos.y;

    mouseDown = true;
  }

  // Right click = move or fire
  if (key.id == CL_MOUSE_RIGHT) {
    std::list<TankVehicle *>::iterator it;
    for (it = tanks.begin(); it != tanks.end(); ++it) {
      TankVehicle *tank1 = (*it);

      if (tank1->isSelected()) {
        bool fire = false;

        // Force firing
        if (key.shift) {
          if (key.ctrl)	// Nuke when ctrl and shift is pressed
            tank1->fire (true);
          else
            tank1->fire (false);

          fire = true;
        } else {
          // Check if clicked on other tank
          std::list<TankVehicle *>::iterator it2;
          for (it2 = tanks.begin(); it2 != tanks.end(); ++it2) {
            TankVehicle *tank2 = (*it2);

            if (tank1 != tank2 && tank2->hitCheck (key.mouse_pos.x, key.mouse_pos.y)) {
              tank1->fire (false);
              fire = true;
            }
          }
        }

        // No fire, move unit
        if (fire == false)
          tank1->setTargetPos (key.mouse_pos.x, key.mouse_pos.y);
      }
    }
  }
}

void World::onMouseUp (const CL_InputEvent &key, const CL_InputState &state) {
  if (key.id == CL_MOUSE_LEFT) {
    // Set end of drag area
    dragArea.right = key.mouse_pos.x;
    dragArea.bottom = key.mouse_pos.y;

    dragArea.normalize();

    // Select/deselect units
    std::list<TankVehicle *>::iterator it;
    for (it = tanks.begin(); it != tanks.end(); ++it) {
      TankVehicle *tank = (*it);

      if (tank->hitCheck (dragArea))
        tank->select();
      else if (!key.shift)
        tank->deselect();
    }

    dragging = false;
  }

  mouseDown = false;
}

void World::onMouseMove (const CL_InputEvent &key, const CL_InputState &state) {
  // Expand drag area
  if (mouseDown) {
    dragging = true;

    dragArea.right = key.mouse_pos.x;
    dragArea.bottom = key.mouse_pos.y;
  }

  // Make turrets target mousepos
  std::list<TankVehicle *>::iterator it;
  for (it = tanks.begin(); it != tanks.end(); ++it) {
    TankVehicle *tank = (*it);

    if (tank->isSelected())
      tank->setTurretTargetPos (key.mouse_pos.x, key.mouse_pos.y);
  }
}

void World::run() {
  while (!window.get_ic().get_keyboard().get_keycode (CL_KEY_ESCAPE)) {
    if (quit)
      break;

    update();
    draw();

    window.flip (1);
    CL_KeepAlive::process();
  };
}

void World::update() {
//  CL_Console::write_line ("cross x/y, center x/y: %1/%2, %3/%4\n", cross_x, cross_y, center_x, center_y);

  int timeElapsed_ms = calcTimeElapsed();
  const int min_border = 200;
  const int min_x = min_border;
  const int min_y = min_border;
  const int max_x = window_width - min_border - 1;
  const int max_y = window_height - min_border - 1;

  const int max_center_x = width - window_width - 1;
  const int max_center_y = height - window_height - 1;

  int move_x = 0;
  int move_y = 0;

  // Move camera
  if (moving_down)
    move_y += timeElapsed_ms*cross_speed;
  if (moving_up)
    move_y -= timeElapsed_ms*cross_speed;
  if (moving_left)
    move_x -= timeElapsed_ms*cross_speed;
  if (moving_right)
    move_x += timeElapsed_ms*cross_speed;

  if (move_y < 0) {
    if (cross_y > min_y) {
      cross_y += move_y;
      move_y = 0;
      if (cross_y < min_y) {
        move_y = cross_y - min_y;
        cross_y = min_y;
      }
    }

    if (move_y < 0 && center_y > 0) {
      center_y += move_y;
      move_y = 0;
      if (center_y < 0) {
        move_y = center_y - 0;
        center_y = 0;
      }
    }

    if (move_y < 0) {
      cross_y += move_y;
      if (cross_y < 0)
        cross_y = 0;
    }
  }

  if (move_x < 0) {
    if (cross_x > min_x) {
      cross_x += move_x;
      move_x = 0;
      if (cross_x < min_x) {
        move_x = cross_x - min_x;
        cross_x = min_x;
      }
    }

    if (move_x < 0 && center_x > 0) {
      center_x += move_x;
      move_x = 0;
      if (center_x < 0) {
        move_x = center_x - 0;
        center_x = 0;
      }
    }

    if (move_x < 0) {
      cross_x += move_x;
      if (cross_x < 0)
        cross_x = 0;
    }
  }


  if (move_y > 0) {
    if (cross_y < max_y) {
      cross_y += move_y;
      move_y = 0;
      if (cross_y > max_y) {
        move_y = cross_y - max_y;
        cross_y = max_y;
      }
    }


    if (move_y > 0 && center_y < max_center_y) {
      center_y += move_y;
      move_y = 0;
      if (center_y > max_center_y) {
        move_y = center_y - max_center_y;
        center_y = max_center_y;
      }
    }

    if (move_y > 0) {
      cross_y += move_y;
      if (cross_y > window_height - 1)
        cross_y = window_height -1;
    }
  }

  if (move_x > 0) {
    if (cross_x < max_x) {
      cross_x += move_x;
      move_x = 0;
      if (cross_x > max_x) {
        move_x = cross_x - max_x;
        cross_x = max_x;
      }
    }


    if (move_x > 0 && center_x < max_center_x) {
      center_x += move_x;
      move_x = 0;
      if (center_x > max_center_x) {
        move_x = center_x - max_center_x;
        center_x = max_center_x;
      }
    }

    if (move_x > 0) {
      cross_x += move_x;
      if (cross_x > window_width - 1)
        cross_x = window_width -1;
    }
  }


  // Update all gameobjects
  std::list<GameObject *>::iterator it;
  for (it = objects.begin(); it != objects.end();) {
    // If update returns false, object should be deleted
    if ( (*it)->update (timeElapsed_ms) == false) {
      delete (*it);
      it = objects.erase (it);
    } else
      ++it;
  }
}

// Calculate amount of time since last frame
int World::calcTimeElapsed() {
  static unsigned int lastTime = 0;

  unsigned int newTime = CL_System::get_time();
  if (lastTime == 0)
    lastTime = newTime;

  int deltaTime = (newTime - lastTime);
  lastTime = newTime;

  return deltaTime;
}

void World::draw() {
  // Draw background
  background->draw (gc, -center_x, -center_y);

  // Draw all gameobjects
  std::list<GameObject *>::iterator it;
  for (it = objects.begin(); it != objects.end(); ++it)
    (*it)->draw (center_x, center_y);

  // Draw cross
  cross->set_scale (0.5, 0.5);
  cross->draw (gc, cross_x, cross_y);
  cross->set_scale (0.25, 0.25);
  cross->draw (gc, cross_x, cross_y);
}

void World::on_window_close() {
  quit = true;
}

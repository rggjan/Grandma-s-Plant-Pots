// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./world.h"

#include <ClanLib/core.h>
#include <list>

#include "./background.h"
#include "./fly.h"
#include "./player.h"

World::World(std::vector<CL_DisplayWindow*> windows)
  : quit(false),
    default_gc(windows[0]->get_gc()) {

  num_players = windows.size();

  // Setup resources
  resources = CL_ResourceManager("resources.xml");
  background = new CL_Sprite(default_gc, "Background", &resources);
  cross = new CL_Sprite(default_gc, "Cross", &resources);

  for (int i=0; i<num_players; i++) {
    Player *player = new Player(windows[i], background->get_width(), background->get_height());
    players.push_back(player);

    slotQuit[i] = players[i]->display_window->sig_window_close()
                  .connect(this, &World::on_window_close);
  }

  for (int i=0; i<num_players; i++) {
    slotKeyDown[i] = players[i]->display_window->get_ic().get_keyboard().
                     sig_key_down().connect(this, &World::onKeyDown);
    slotKeyUp[i] = players[i]->display_window->get_ic().get_keyboard().
                   sig_key_up().connect(this, &World::onKeyUp);
  }
  /*

    slotMouseDown = windows[0]->get_ic().get_mouse().sig_key_down().
                      connect(this, &World::onMouseDown);
                        slotMouseDown = windows[0]->get_ic().get_keyboard().sig_key_down().

                      connect(this, &World::onMouseDown);
      slotMouseUp = windows[0]->get_ic().get_mouse().sig_key_up().
                    connect(this, &World::onMouseUp);
      slotMouseMove = windows[0]->get_ic().get_mouse().sig_pointer_move().
                      connect(this, &World::onMouseMove);
    */

  dragging = mouseDown = false;

  // Add some objects
  initLevel();

  // Run the main loop
  run();
}

World::~World() {
  // Delete all gameobjects
  std::list<GameObject *>::iterator it;
  for (it = objects.begin(); it != objects.end(); ++it)
    delete(*it);
}

void World::initLevel() {
  for (int i = 0; i < 10; i++) {
    Fly *fly = new Fly(this, &default_gc);
    fly->setPos(i*10, i*10);
    addFly(fly);
  }
}

void World::addObject(GameObject *object) {
  objects.push_back(object);
}

void World::addFly(Fly *tank) {
  objects.push_back(tank);
  flies.push_back(tank);
}

bool World::hitCheck(CL_CollisionOutline *outline, GameObject *other) {
  std::list<GameObject *>::iterator it;
  for (it = objects.begin(); it != objects.end(); ++it) {
    if ((*it) != other) {
      if ((*it)->hitCheck(outline))
        return true;
    }
  }

  return false;
}

void World::onKeyDown(const CL_InputEvent &key, const CL_InputState &state) {
  if (key.id == CL_KEY_DOWN) {
    players[0]->moving_down = true;
  }

  if (key.id == CL_KEY_UP) {
    players[0]->moving_up = true;
  }

  if (key.id == CL_KEY_LEFT) {
    players[0]->moving_left = true;
  }

  if (key.id == CL_KEY_RIGHT) {
    players[0]->moving_right = true;
  }
}

void World::onKeyUp(const CL_InputEvent &key, const CL_InputState &state) {
  if (key.id == CL_KEY_DOWN) {
    players[0]->moving_down = false;
  }

  if (key.id == CL_KEY_UP) {
    players[0]->moving_up = false;
  }

  if (key.id == CL_KEY_LEFT) {
    players[0]->moving_left = false;
  }

  if (key.id == CL_KEY_RIGHT) {
    players[0]->moving_right = false;
  }
}

void World::onMouseDown(const CL_InputEvent &key, const CL_InputState &state) {
  std::cout << "mouse" << std::endl;
  // Start dragging on left click
  if (key.id == CL_MOUSE_LEFT) {
    dragArea.left = key.mouse_pos.x;
    dragArea.top = key.mouse_pos.y;

    mouseDown = true;
  }
}

void World::onMouseUp(const CL_InputEvent &key, const CL_InputState &state) {
  if (key.id == CL_MOUSE_LEFT) {
    // Set end of drag area
    dragArea.right = key.mouse_pos.x;
    dragArea.bottom = key.mouse_pos.y;

    dragArea.normalize();

    dragging = false;
  }

  mouseDown = false;
}

void World::onMouseMove(const CL_InputEvent &key, const CL_InputState &state) {
  // Expand drag area
  if (mouseDown) {
    dragging = true;

    dragArea.right = key.mouse_pos.x;
    dragArea.bottom = key.mouse_pos.y;
  }
}

void World::run() {
  while (!players[0]->display_window->get_ic().get_keyboard().get_keycode(CL_KEY_ESCAPE)) {
    if (quit)
      break;

    update();
    draw();

    for (int i=0; i<num_players; i++) {
      players[i]->display_window->flip(0);
      if (i == num_players-1) {
        players[i]->display_window->flip(1);
      }
    }

    CL_KeepAlive::process();
  }
}

void World::update() {
  int timeElapsed_ms = calcTimeElapsed();

  for (int i=0; i<num_players; i++) {
    players[i]->update(timeElapsed_ms);
  }

  // Make turrets target mousepos
  std::list<Fly *>::iterator fly_it;
  for (fly_it = flies.begin(); fly_it != flies.end(); ++fly_it) {
    Fly *fly = (*fly_it);

    fly->setTargetPos(players[0]->cross_x + players[0]->center_x, players[0]->cross_y + players[0]->center_y);
  }

  // Update all gameobjects
  std::list<GameObject *>::iterator it;
  for (it = objects.begin(); it != objects.end();) {
    // If update returns false, object should be deleted
    if ((*it)->update(timeElapsed_ms) == false) {
      delete(*it);
      it = objects.erase(it);
    } else {
      ++it;
    }
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
  for (int i=0; i<num_players; i++) {
    background->draw(*(players[i]->gc),
                     -players[i]->center_x,
                     -players[i]->center_y);

    // Draw all gameobjects
    std::list<GameObject *>::iterator it;
    for (it = objects.begin(); it != objects.end(); ++it)
      (*it)->draw(*(players[i]->gc), players[i]->center_x, players[i]->center_y);

    // Draw cross
    cross->set_scale(0.5, 0.5);
    cross->draw(*(players[i]->gc), players[i]->cross_x, players[i]->cross_y);
    cross->set_scale(0.25, 0.25);
    cross->draw(*(players[i]->gc), players[i]->cross_x, players[i]->cross_y);
  }

  /*
    CL_PixelBuffer buffer(256, 256, cl_rgba8);
    unsigned int *pixel_data = (unsigned int *) buffer.get_data();
    CL_BezierCurve curve;
    curve.add_control_point(0, 50);
    curve.add_control_point(50, 50);
    curve.add_control_point(50, 80);

    CL_Pointf old_point(0, 0);

    for (int p = 0; p < 100; p++) {
      unsigned char red = 0;
      unsigned char green = 255;
      unsigned char blue = 0;
      unsigned char alpha = 255;

      CL_Pointf point = curve.get_point_relative(p/100.);
      CL_Pointf direction = point-old_point;
      direction.normalize();
      //direction.x
      direction.rotate(CL_Vec2f(0, 0), CL_Angle(90, cl_degrees));
      int max_dist = p/10.+5.;

      for (int dist=0; dist<max_dist; dist++) {
        CL_Pointf current = point + direction*dist;
        green = (max_dist-dist)/15.*255;
        int x = current.x;
        int y = current.y;
        pixel_data[x + y * 256] = (red << 24) + (green << 16) + (blue << 8) + alpha;

        current = point - direction*dist;
        green = (max_dist-dist)/15.*255;
        x = current.x;
        y = current.y;
        pixel_data[x + y * 256] = (red << 24) + (green << 16) + (blue << 8) + alpha;
      }

      old_point = point;
    }

    gc.draw_pixels(0, 0, buffer, CL_Rect(0, 0, 256, 256), CL_Colorf(1.0f, 0.0f, 0.0f));*/
}

void World::on_window_close() {
  quit = true;
}

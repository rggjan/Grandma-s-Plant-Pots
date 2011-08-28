// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./world.h"

#include <ClanLib/core.h>
#include <ClanLib/sound.h>
#include <list>
#include <vector>

#include "./background.h"
#include "./fly.h"
#include "./player.h"
#include "./flower.h"
#include "./leaf.h"
#include "./plantplayer.h"
#include "./bugplayer.h"

#define BACKGROUND_BORDER 65

World::World(std::vector<CL_DisplayWindow*> windows)
  : quit(false),
    default_gc(windows[0]->get_gc()) {
  num_players = windows.size();

  // Setup resources
  resources = CL_ResourceManager("resources.xml");
  background = new CL_Sprite(default_gc, "Background", &resources);
  width = background->get_width();
  height = background->get_height();
  if (width != height)
    CL_Console::write_line("Error, height and width should be the same!");

  for (int i = 0; i < num_players; i++) {
    Player *player;

    if (i % 2 == 0) {
      player = new BugPlayer(windows[i], this, width, height);
    } else {
      player = new PlantPlayer(windows[i], this, width, height);
    }
    players.push_back(player);

    slotQuit[i] = players[i]->display_window->sig_window_close()
                  .connect(this, &World::on_window_close);
  }

  for (int i = 0; i < num_players; i++) {
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
  // TODO(rggjan): real cleanup of everything
  std::vector<GameObject *>::iterator it;
  for (it = objects.begin(); it != objects.end(); ++it)
    delete(*it);
}

void World::initLevel() {
  for (int i = 0; i < 10; i++) {
    Fly *fly = new Fly(this, default_gc, "SpaceShootTurretShooting");
    fly->set_position(CL_Vec2f(i * 10, i * 10));
    addFly(fly);
  }
  for (int i = 0; i < 10; i++) {
    Fly *fly = new Fly(this, default_gc, "Bug2");
    fly->set_position(CL_Vec2f(i * 60, i * 60));
    addFly(fly);
  }
}

bool World::CanBuild(CL_Vec2f position) {
  CL_Vec2f diff = CL_Vec2f(width / 2, height / 2) - position;
  return diff.length() < (height / 2 - BACKGROUND_BORDER);
}

void World::addObject(GameObject *object) {
  objects.push_back(object);
}

void World::addFly(Fly *tank) {
  objects.push_back(tank);
  flies.push_back(tank);
}

void World::addFlower(Flower *flower) {
  objects.push_back(flower);
  flowers.push_back(flower);
}

void World::addLeaf(Leaf *leaf) {
  objects.push_back(leaf);
}

Flower* World::NearestFlower(CL_Vec2f position) {
  // TODO(rggjan): infinity
  int best_dist = -1;
  Flower *nearest_flower = NULL;

  // Get nearest flower
  std::vector<Flower *>::iterator it;
  for (it = flowers.begin(); it != flowers.end(); ++it) {
    float distance = ((*it)->position() - position).length();

    if (nearest_flower == NULL || distance < best_dist) {
      best_dist = distance;
      nearest_flower = (*it);
    }
  }

  return nearest_flower;
}

void World::onKeyDown(const CL_InputEvent &key, const CL_InputState &state) {
  if (key.id == CL_KEY_ESCAPE)
    quit = true;

  // key Player 0 onKeyDown
  if (num_players > 0) {
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
    if (key.id == CL_KEY_DELETE) {
      players[0]->BuildButtonPressed();
    }

    if (key.id == CL_KEY_END) {
      players[0]->SelectButtonPressed();
    }

    if (key.id == CL_KEY_HOME) {
      players[0]->CancelButtonPressed();
    }
  }
  // key Player 1 onKeyDown
  if (num_players > 1) {
    if (key.id == CL_KEY_NUMPAD2) {
      players[1]->moving_down = true;
    }

    if (key.id == CL_KEY_NUMPAD5) {
      players[1]->moving_up = true;
    }

    if (key.id == CL_KEY_NUMPAD1) {
      players[1]->moving_left = true;
    }

    if (key.id == CL_KEY_NUMPAD3) {
      players[1]->moving_right = true;
    }

    if (key.id == CL_KEY_NUMPAD6) {
      players[1]->BuildButtonPressed();
    }

    if (key.id == CL_KEY_NUMPAD4) {
      players[1]->SelectButtonPressed();
    }

    if (key.id == CL_KEY_NUMPAD0) {
      players[1]->CancelButtonPressed();
    }
  }
  // key Player 2 onKeyDown
  if (num_players > 2) {
    if (key.id == CL_KEY_J) {
      players[2]->moving_down = true;
    }

    if (key.id == CL_KEY_U) {
      players[2]->moving_up = true;
    }

    if (key.id == CL_KEY_H) {
      players[2]->moving_left = true;
    }

    if (key.id == CL_KEY_K) {
      players[2]->moving_right = true;
    }
    if (key.id == CL_KEY_7) {
      players[2]->BuildButtonPressed();
    }

    if (key.id == CL_KEY_8) {
      players[2]->SelectButtonPressed();
    }

    if (key.id == CL_KEY_9) {
      players[2]->CancelButtonPressed();
    }
  }
  // key Player 3 onKeyDown
  if (num_players > 3) {
    if (key.id == CL_KEY_S) {
      players[3]->moving_down = true;
    }

    if (key.id == CL_KEY_W) {
      players[3]->moving_up = true;
    }

    if (key.id == CL_KEY_A) {
      players[3]->moving_left = true;
    }

    if (key.id == CL_KEY_D) {
      players[3]->moving_right = true;
    }
    if (key.id == CL_KEY_1) {
      players[3]->BuildButtonPressed();
    }

    if (key.id == CL_KEY_2) {
      players[3]->SelectButtonPressed();
    }

    if (key.id == CL_KEY_3) {
      players[3]->CancelButtonPressed();
    }
  }
  // key Player 4 onKeyDown
  if (num_players > 4) {
    if (key.id == CL_KEY_DOWN) {
      players[4]->moving_down = true;
    }

    if (key.id == CL_KEY_UP) {
      players[4]->moving_up = true;
    }

    if (key.id == CL_KEY_LEFT) {
      players[4]->moving_left = true;
    }

    if (key.id == CL_KEY_RIGHT) {
      players[4]->moving_right = true;
    }

    if (key.id == CL_KEY_NUMPAD6) {
      players[4]->BuildButtonPressed();
    }

    if (key.id == CL_KEY_NUMPAD4) {
      players[4]->SelectButtonPressed();
    }

    if (key.id == CL_KEY_NUMPAD0) {
      players[4]->CancelButtonPressed();
    }
  }
}
void World::onKeyUp(const CL_InputEvent &key, const CL_InputState &state) {
  // key Player 0 onKeyUp
  if (num_players > 0) {
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
  // key Player 1 onKeyUp
  if (num_players > 1) {
    if (key.id == CL_KEY_NUMPAD2) {
      players[1]->moving_down = false;
    }

    if (key.id == CL_KEY_NUMPAD5) {
      players[1]->moving_up = false;
    }

    if (key.id == CL_KEY_NUMPAD1) {
      players[1]->moving_left = false;
    }

    if (key.id == CL_KEY_NUMPAD3) {
      players[1]->moving_right = false;
    }
  }

  // key Player 2 onKeyUp
  if (num_players > 2) {
    if (key.id == CL_KEY_J) {
      players[2]->moving_down = false;
    }

    if (key.id == CL_KEY_U) {
      players[2]->moving_up = false;
    }

    if (key.id == CL_KEY_H) {
      players[2]->moving_left = false;
    }

    if (key.id == CL_KEY_K) {
      players[2]->moving_right = false;
    }
  }
  // key Player 3 onKeyUp
  if (num_players > 3) {
    if (key.id == CL_KEY_S) {
      players[3]->moving_down = false;
    }

    if (key.id == CL_KEY_W) {
      players[3]->moving_up = false;
    }

    if (key.id == CL_KEY_A) {
      players[3]->moving_left = false;
    }

    if (key.id == CL_KEY_D) {
      players[3]->moving_right = false;
    }
  }
  // key Player 4 onKeyUp
  if (num_players > 4) {
    if (key.id == CL_KEY_DOWN) {
      players[4]->moving_down = false;
    }

    if (key.id == CL_KEY_UP) {
      players[4]->moving_up = false;
    }

    if (key.id == CL_KEY_LEFT) {
      players[4]->moving_left = false;
    }

    if (key.id == CL_KEY_RIGHT) {
      players[4]->moving_right = false;
    }
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
  CL_SoundBuffer *sound = new CL_SoundBuffer("BackgroundMusic", &resources);
  sound->set_volume(1.0f);
  sound->prepare();
  sound->play();

  while (!quit) {
    update();
    draw();

    for (int i = 0; i < num_players; i++) {
      players[i]->display_window->flip(0);
      if (i == num_players - 1) {
        players[i]->display_window->flip(1);
      }
    }

    CL_KeepAlive::process();
  }
}

void World::update() {
  int timeElapsed_ms = calcTimeElapsed();

  for (int i = 0; i < num_players; i++) {
    players[i]->Update(timeElapsed_ms);
  }

  // Make turrets target mousepos
  std::vector<Fly *>::iterator fly_it;
  for (fly_it = flies.begin(); fly_it != flies.end(); ++fly_it) {
    Fly *fly = (*fly_it);

    fly->set_target_position(players[0]->position());
    fly->update(timeElapsed_ms);
  }
  /*
    // Update all gameobjects
    std::vector<GameObject *>::iterator it;
    for (it = objects.begin(); it != objects.end();) {
      // If update returns false, object should be deleted
      if ((*it)->update(timeElapsed_ms) == false) {
        delete(*it);
        it = objects.erase(it);
      } else {
        ++it;
      }
    }*/
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
  for (int i = 0; i < num_players; i++) {
    background->draw(*(players[i]->gc),
                     -players[i]->map_position_.x,
                     -players[i]->map_position_.y);

    // Draw all gameobjects
    // Flowers
    std::vector<Flower *>::iterator it1;
    for (it1 = flowers.begin(); it1 != flowers.end(); ++it1)
      (*it1)->Draw(players[i]->gc, players[i]->map_position());

    // Flies
    std::vector<Fly *>::iterator it2;
    for (it2 = flies.begin(); it2 != flies.end(); ++it2)
      (*it2)->Draw(players[i]->gc, players[i]->map_position());

    players[i]->Draw();
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

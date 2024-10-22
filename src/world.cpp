// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./world.h"

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/sound.h>
#include <algorithm>
#include <vector>
#include <list>

#include "plants/flower.h"
#include "plants/plantplayer.h"
#include "bugs/bugplayer.h"

#define BACKGROUND_BORDER 65
#define NUM_PLAYERS 4

using std::list;
using std::sort;

World::World(clan::DisplayWindow window)
  : quit(false),
    canvas_(window),
    time_elapsed_ms_(1),
    fps_(0),
    window_(window) {
  num_players = NUM_PLAYERS;

  // Setup resources
  resources = clan::XMLResourceManager::create(clan::XMLResourceDocument("resources.xml"));
  clan::Sprite background = clan::Sprite::resource(canvas_, "Background", resources);
  width = background.get_width();
  height = background.get_height();
  if (width != height)
    clan::Console::write_line("Error, height and width should be the same!");

  player_width_ = canvas_.get_width() / 2;
  player_height_ = canvas_.get_height() / ((num_players+1)/2);

  for (int i = 0; i < num_players; i++) {
    Player *player;

    if (i % 2 == 0) {
      player = new BugPlayer(canvas_, this, background, clan::Size(player_width_, player_height_));
    } else {
      player = new PlantPlayer(canvas_, this, background, clan::Size(player_width_, player_height_));
    }
    players.push_back(player);
  }

  slotQuit[0] = window.sig_window_close()
                .connect(this, &World::on_window_close);
  slotKeyDown[0] = window.get_ic().get_keyboard().
                   sig_key_down().connect(this, &World::onKeyDown);
  slotKeyUp[0] = window.get_ic().get_keyboard().
                 sig_key_up().connect(this, &World::onKeyUp);

  clan::FontDescription desc;
  desc.set_height(20);
  desc.set_typeface_name("Tahoma");
  default_font_ = clan::Font(canvas_, desc);

  // Setup sounds
  sound_beep1_ = clan::SoundBuffer::resource("Beep1Music", resources);
  sound_beep1_.set_volume(0.1f);
  sound_beep1_.prepare();

  // Run the main loop
  Run();
}

World::~World() {
    // Delete all gameobjects
    // TODO(rggjan): real cleanup of everything
}

void World::PlayBeep() {
    sound_beep1_.play();
}

bool World::CanBuild(clan::Vec2f position) {
    clan::Vec2f diff = clan::Vec2f(width / 2.0f, height / 2.0f) - position;
    return diff.length() < (height / 2 - BACKGROUND_BORDER);
}

void World::AddBug(Bug *bug) {
    bugs.push_back(bug);
}

void World::RemoveBug(Bug *bug) {
    bugs.remove(bug);
}

void World::AddPlant(Plant *plant) {
  if (plant->is_master_plant())
    master_plants.push_back(plant);

  plants.push_back(plant);
}

void World::RemovePlant(Plant *plant) {
  if (plant->is_master_plant())
    master_plants.remove(plant);

  plants.remove(plant);
}

struct sort_class {
    bool operator() (GameObject *plant1, GameObject* plant2) {
        return ((plant2->position() - position).length() -
                (plant1->position() - position).length()) > 0;
    }
    clan::Vec2f position;
};

list<Plant *>* World::NearestPlants(clan::Vec2f position) {
    sort_class sort_object;
    sort_object.position = position;

    plants.sort(sort_object);
    return &plants;
}

list<Bug*>* World::NearestBugs(clan::Vec2f position) {
    sort_class sort_object;
    sort_object.position = position;

    bugs.sort(sort_object);
    return &bugs;
}

Plant* World::NearestMasterPlant(clan::Vec2f position) {
    // TODO(rggjan): infinity
    float best_dist = -1;
    Plant *nearest_plant = NULL;

    // Get nearest flower
    std::list<Plant *>::iterator it;
    for (it = master_plants.begin(); it != master_plants.end(); ++it) {
        if (!(*it)->is_alive())
            continue;

        float distance = ((*it)->position() - position).length();

        if (nearest_plant == NULL || distance < best_dist) {
            best_dist = distance;
            nearest_plant = (*it);
        }
    }

    return nearest_plant;
}

void World::onKeyDown(const clan::InputEvent &key) {
    if (key.id == clan::keycode_escape) {
        quit = true;
    exit(0);
      }


    // key Player 0 onKeyDown
    if (num_players > 0) {
        switch (key.id) {
        case clan::keycode_down:
            players[0]->MovingDownButtonPressed();
            break;
        case clan::keycode_up:
            players[0]->MovingUpButtonPressed();
            break;
        case clan::keycode_left:
            players[0]->MovingLeftButtonPressed();
            break;
        case clan::keycode_right:
            players[0]->MovingRightButtonPressed();
            break;
        case clan::keycode_delete:
            players[0]->BuildButtonPressed();
            break;
        case clan::keycode_end:
            players[0]->SelectButtonPressed();
            break;
        case clan::keycode_home:
            players[0]->CancelButtonPressed();
            break;
        default:
            break;
        }
    }

    // key Player 1 onKeyDown
    if (num_players > 1) {
        switch (key.id) {
        case clan::keycode_numpad2:
            players[1]->MovingDownButtonPressed();
            break;
        case clan::keycode_numpad5:
            players[1]->MovingUpButtonPressed();
            break;
        case clan::keycode_numpad1:
            players[1]->MovingLeftButtonPressed();
            break;
        case clan::keycode_numpad3:
            players[1]->MovingRightButtonPressed();
            break;
        case clan::keycode_numpad6:
            players[1]->BuildButtonPressed();
            break;
        case clan::keycode_numpad4:
            players[1]->SelectButtonPressed();
            break;
        case clan::keycode_numpad0:
            players[1]->CancelButtonPressed();
            break;
        default:
            break;
        }
    }

    // key Player 2 onKeyDown
    if (num_players > 2) {
        switch (key.id) {
        case clan::keycode_j:
            players[2]->MovingDownButtonPressed();
            break;
        case clan::keycode_u:
            players[2]->MovingUpButtonPressed();
            break;
        case clan::keycode_h:
            players[2]->MovingLeftButtonPressed();
            break;
        case clan::keycode_k:
            players[2]->MovingRightButtonPressed();
            break;
        case clan::keycode_i:
            players[2]->BuildButtonPressed();
            break;
        case clan::keycode_z:
            players[2]->SelectButtonPressed();
            break;
        case clan::keycode_n:
            players[2]->CancelButtonPressed();
        default:
            break;
        }
    }

    // key Player 3 onKeyDown
    if (num_players > 3) {
        switch (key.id) {
        case clan::keycode_s:
            players[3]->MovingDownButtonPressed();
            break;
        case clan::keycode_w:
            players[3]->MovingUpButtonPressed();
            break;
        case clan::keycode_a:
            players[3]->MovingLeftButtonPressed();
            break;

        case clan::keycode_d:
            players[3]->MovingRightButtonPressed();
            break;
        case clan::keycode_e:
            players[3]->BuildButtonPressed();
            break;
        case clan::keycode_q:
            players[3]->SelectButtonPressed();
            break;

        case clan::keycode_y:
            players[3]->CancelButtonPressed();
            break;
        default:
            break;
        }
    }
}

void World::onKeyUp(const clan::InputEvent &key) {
    // key Player 0 onKeyUp
    if (num_players > 0) {
        switch (key.id) {
        case clan::keycode_down:
            players[0]->MovingDownButtonReleased();
            break;
        case clan::keycode_up:
            players[0]->MovingUpButtonReleased();
            break;
        case clan::keycode_left:
            players[0]->MovingLeftButtonReleased();
            break;

        case clan::keycode_right:
            players[0]->MovingRightButtonReleased();
            break;
        default:
            break;
        }
    }

    // key Player 1 onKeyUp
    if (num_players > 1) {
        switch (key.id) {
        case clan::keycode_numpad2:
            players[1]->MovingDownButtonReleased();
            break;
        case clan::keycode_numpad5:
            players[1]->MovingUpButtonReleased();
            break;
        case clan::keycode_numpad1:
            players[1]->MovingLeftButtonReleased();
            break;
        case clan::keycode_numpad3:
            players[1]->MovingRightButtonReleased();
            break;
        default:
            break;
        }
    }

    // key Player 2 onKeyUp
    if (num_players > 2) {
        switch (key.id) {
        case clan::keycode_j:
            players[2]->MovingDownButtonReleased();
            break;

        case clan::keycode_u:
            players[2]->MovingUpButtonReleased();
            break;
        case clan::keycode_h:
            players[2]->MovingLeftButtonReleased();
            break;
        case clan::keycode_k:
            players[2]->MovingRightButtonReleased();
            break;
        default:
            break;
        }
    }

    // key Player 3 onKeyUp
    if (num_players > 3) {
        switch (key.id) {
        case clan::keycode_s:
            players[3]->MovingDownButtonReleased();
            break;
        case clan::keycode_w:
            players[3]->MovingUpButtonReleased();
            break;
        case clan::keycode_a:
            players[3]->MovingLeftButtonReleased();
            break;
        case clan::keycode_d:
            players[3]->MovingRightButtonReleased();
            break;
        default:
            break;
        }
    }
}

void World::Run() {
    clan::SoundBuffer sound = clan::SoundBuffer::resource("BackgroundMusic", resources);
    sound.set_volume(1.0f);
    sound.prepare();
    //sound.play();

    while (!quit) {
        Update();
        Draw();

        window_.flip(0);

        clan::KeepAlive::process();
    }
}

void World::Update() {
    time_elapsed_ms_ = CalcTimeElapsed();
    if (time_elapsed_ms_ > 0)
        fps_ = fps_ * 0.95 + 1000. / time_elapsed_ms_ * 0.05;

    for (int i = 0; i < num_players; i++) {
        players[i]->Update(time_elapsed_ms_);
    }
}

// Calculate amount of time since last frame
clan::ubyte64 World::CalcTimeElapsed() {
    static clan::ubyte64 lastTime = 0;

    clan::ubyte64 newTime = clan::System::get_time();
    if (lastTime == 0)
        lastTime = newTime;

    clan::ubyte64 deltaTime = (newTime - lastTime);
    lastTime = newTime;

    return deltaTime;
}

void World::Draw() {
    for (int i = 0; i < num_players; i++) {
        clan::Point playerPosition((i%2)*player_width_, (i/2)*player_height_);

        canvas_.set_cliprect(clan::Rect(playerPosition, clan::Size(player_width_, player_height_)));
        canvas_.push_translate(playerPosition);

        players[i]->DrawFloor();

        // Draw all gameobjects
        // Plants
        std::list<Plant*>::iterator plant;
        for (plant = master_plants.begin(); plant != master_plants.end(); ++plant) {
            (*plant)->Draw(players[i]->canvas_, players[i]->map_position());
        }

        // Bugs
        std::list<Bug*>::iterator bug;
        for (bug = bugs.begin(); bug != bugs.end(); ++bug) {
            (*bug)->Draw(players[i]->canvas_, players[i]->map_position());
        }

        players[i]->DrawTop();

        canvas_.pop_modelview();
    }

    canvas_.reset_cliprect();

    default_font_.draw_text(canvas_, clan::Pointf(10, 100),
                            clan::string_format("FPS: %1", static_cast<int>(fps_)),
                            clan::Colorf::white);

    // Draw borders
    const float border_width = 6;
    canvas_.fill_rect(clan::Rectf(clan::Pointf(player_width_ - border_width/2, 0), clan::Sizef(border_width, canvas_.get_height())), clan::Colorf::silver);
    canvas_.fill_rect(clan::Rectf(clan::Pointf(0, player_height_ - border_width/2), clan::Sizef(canvas_.get_width(), border_width)), clan::Colorf::silver);

    /*
    clan::PixelBuffer buffer(256, 256, clan::rgba8);
    unsigned int *pixel_data = (unsigned int *) buffer.get_data();
    clan::BezierCurve curve;
    curve.add_control_point(0, 50);
    curve.add_control_point(50, 50);
    curve.add_control_point(50, 80);

    clan::Pointf old_point(0, 0);

    for (int p = 0; p < 100; p++) {
      unsigned char red = 0;
      unsigned char green = 255;
      unsigned char blue = 0;
      unsigned char alpha = 255;

      clan::Pointf point = curve.get_point_relative(p/100.);
      clan::Pointf direction = point-old_point;
      direction.normalize();
      //direction.x
      direction.rotate(clan::Vec2f(0, 0), clan::Angle(90, clan::degrees));
      int max_dist = p/10.+5.;

      for (int dist=0; dist<max_dist; dist++) {
        clan::Pointf current = point + direction*dist;
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

    canvas.draw_pixels(0, 0, buffer, clan::Rect(0, 0, 256, 256), clan::Colorf(1.0f, 0.0f, 0.0f));*/
}

void World::on_window_close() {
    quit = true;
}

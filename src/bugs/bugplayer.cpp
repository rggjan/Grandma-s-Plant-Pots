// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "bugs/bugplayer.h"

#include <ClanLib/sound.h>
#include <ClanLib/core.h>
#include <list>
#include <algorithm>

#include "./world.h"
#include "plants/plant.h"

#define NUM_BUGS 2
#define START_ENERGY 10

using std::list;
using std::count_if;

BugPlayer::BugPlayer(CL_GraphicContext* gc, World* world,
                     int width, int height)
  : Player(gc, world, width, height) {
  selectedImage = new CL_Sprite(*gc_, "Cross2", &world->resources);
  selectedImage->set_alpha(0.5);

  for (int i = 0; i < NUM_BUGS; i++) {
    const char* name;

    if (i % 2 == 0)
      name = "Bug1";
    else
      name = "Bug2";

    CreateBug(name, CL_Vec2f(i * 60, i * 60));
  }

  sound_bug_attack_ = new CL_SoundBuffer("BugAttack", &world->resources);
  sound_bug_attack_->set_volume(0.1f);
  sound_bug_attack_->prepare();
}

void BugPlayer::CreateBug(CL_StringRef name, CL_Vec2f position) {
  AddBug(new Bug(world_, gc_, position, name, this));
}

void BugPlayer::AddBug(Bug* bug) {
  bugs.push_back(bug);
}

void BugPlayer::SelectButtonPressed() {
  Bug* bug = GetFreeBug();

  if (bug != NULL && nearest_free_plant_ != NULL) {
    sound_bug_attack_->play();
    bug->SetTargetPlant(nearest_free_plant_);
  } else {
    world_->PlayBeep();
  }
}

void BugPlayer::CancelButtonPressed() {
  for_each(bugs.begin(), bugs.end(), [](Bug* bug) { bug->StopEating(); });
}

void BugPlayer::BuildButtonPressed() {
}

Plant* BugPlayer::GetFreePlant() {
  for (Plant *plant : *world_->NearestPlants(position()))
    if (plant->is_alive() && plant->free_space() && Visible(plant->position()))
      return plant;

  return NULL;
}

Bug* BugPlayer::GetFreeBug() {
  std::list<Bug *>::iterator it;
  for (it = bugs.begin(); it != bugs.end(); ++it) {
    if ((*it)->is_alive() && (*it)->is_free())
      return *it;
  }

  return NULL;
}

void BugPlayer::DrawFloor() {
  if (nearest_free_plant_ != NULL) {
    CL_Vec2f pos = nearest_free_plant_->position() - map_position();
    selectedImage->draw(*gc_, pos.x, pos.y);
  }
  /*  } else if (state == Selected || state == SelectedBuilding) {
      selectedImage->set_alpha(0.8);

      CL_Vec2f pos = selectedFlower->position() - map_position();
      selectedImage->draw(*gc, pos.x, pos.y);
    }*/
}

bool IsAlive(Bug *bug) { return bug->is_alive(); }

void BugPlayer::DrawTop() {
  int size = count_if(bugs.begin(), bugs.end(), IsAlive);

  CL_Colorf color = CL_Colorf::white;
  default_font_.draw_text(*gc_, CL_Pointf(10, 30),
                         cl_format("Bugs: %1", size), color);
  Player::DrawTop();
}

void BugPlayer::Update(int time_ms) {
  std::list<Bug *>::iterator it;
  for (it = bugs.begin(); it != bugs.end();) {
    Bug* bug = *it;
    bug->set_target_position(position());

    if (!bug->Update(time_ms)) {
      delete bug;
      it = bugs.erase(it);
    } else {
      ++it;
    }
  }

  nearest_free_plant_ = GetFreePlant();

  Player::Update(time_ms);
}

// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "bugs/bugplayer.h"

#include <ClanLib/sound.h>
#include <ClanLib/core.h>
#include <vector>

#include "./world.h"
#include "plants/plant.h"

#define NUM_BUGS 2
#define START_ENERGY 10

using std::vector;

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
  Bug * bug = new Bug(world, *gc_, name, this);
  bug->set_position(position);

  AddBug(bug);
}

void BugPlayer::AddBug(Bug* bug) {
  bugs.push_back(bug);
  world->AddBug(bug);
}

void BugPlayer::SelectButtonPressed() {
  Bug* bug = GetFreeBug();

  if (bug != NULL && nearest_free_plant_ != NULL) {
    sound_bug_attack_->play();
    bug->SetTargetPlant(nearest_free_plant_);
  } else {
    world->PlayBeep();
  }
}

void BugPlayer::CancelButtonPressed() {
  int size = bugs.size();
  for (int i = 0; i < size; i++) {
    bugs[i]->StopEating();
  }
}

void BugPlayer::BuildButtonPressed() {
}

Plant* BugPlayer::GetFreePlant() {
  vector<Plant*> *plants = world->NearestPlants(position());

  int size = plants->size();
  for (int i = 0; i < size; i++) {
    Plant* plant = (*plants)[i];
    if (plant->is_alive() && plant->free_space() && Visible(plant->position()))
      return (*plants)[i];
  }

  return NULL;
}

Bug* BugPlayer::GetFreeBug() {
  int size = bugs.size();
  for (int i = 0; i < size; i++) {
    if (bugs[i]->is_alive() && bugs[i]->is_free()) {
      return bugs[i];
    }
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

void BugPlayer::DrawTop() {
  int size=0;
  for (unsigned int i=0; i<bugs.size(); i++) {
    size += bugs[i]->is_alive();
  }

  CL_Colorf color = CL_Colorf::white;
    default_font.draw_text(*gc_, CL_Pointf(10, 30),
                           cl_format("Bugs: %1", size), color);
  Player::DrawTop();
}

void BugPlayer::Update(int time_ms) {
  int size = bugs.size();
  for (int i = 0; i < size; i++) {
    Bug *bug = bugs[i];

    bug->set_target_position(position());
    bug->update(time_ms);
  }

  nearest_free_plant_ = GetFreePlant();  

  Player::Update(time_ms);
}

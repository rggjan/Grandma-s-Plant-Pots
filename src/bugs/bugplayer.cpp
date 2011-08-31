// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "bugs/bugplayer.h"

#include <vector>

#include "./world.h"
#include "plants/plant.h"

#define NUM_BUGS 2

using std::vector;

BugPlayer::BugPlayer(CL_DisplayWindow* window, World* world,
                     int width, int height)
  : Player(window, world, width, height) {
  selectedImage = new CL_Sprite(*gc, "Cross2", &world->resources);
  selectedImage->set_alpha(0.5);

  for (int i = 0; i < NUM_BUGS; i++) {
    const char* name;

    if (i%2 == 0)
      name = "Bug1";
    else
      name = "Bug2";

    CreateFly(name, CL_Vec2f(i*60, i*60));
  }
}

void BugPlayer::CreateFly(CL_StringRef name, CL_Vec2f position) {
    Fly * fly = new Fly(world, *gc, name, this);
    fly->set_position(position);

    AddFly(fly);
}

void BugPlayer::AddFly(Fly* fly) {
  flies.push_back(fly);
  world->AddFly(fly);
}

void BugPlayer::SelectButtonPressed() {
  Fly* fly = GetFreeBug();

  if (fly != NULL && nearest_free_plant_ != NULL)
    fly->SetTargetPlant(nearest_free_plant_);
  // else
  // TODO(rggjan): beep
}

void BugPlayer::CancelButtonPressed() {
  int size = flies.size();
  for (int i = 0; i < size; i++) {
    flies[i]->StopEating();
  }
}

void BugPlayer::BuildButtonPressed() {
}

Plant* BugPlayer::GetFreePlant() {
  vector<Plant*> *plants = world->NearestPlants(position());

  int size = plants->size();
  for (int i = 0; i < size; i++) {
    if ((*plants)[i]->free_space())
      return (*plants)[i];
  }

  return NULL;
}

Fly* BugPlayer::GetFreeBug() {
  int size = flies.size();
  for (int i = 0; i < size; i++) {
    if (flies[i]->is_free()) {
      return flies[i];
    }
  }

  return NULL;
}

void BugPlayer::DrawFloor() {
  nearest_free_plant_ = GetFreePlant();

  if (nearest_free_plant_ != NULL) {
    CL_Vec2f pos = nearest_free_plant_->position() - map_position();
    selectedImage->draw(*gc, pos.x, pos.y);
  }
  /*  } else if (state == Selected || state == SelectedBuilding) {
      selectedImage->set_alpha(0.8);

      CL_Vec2f pos = selectedFlower->position() - map_position();
      selectedImage->draw(*gc, pos.x, pos.y);
    }*/
}

void BugPlayer::Update(int time_ms) {
  int size = flies.size();
  for (int i=0; i<size; i++) {
    Fly *fly = flies[i];

    fly->set_target_position(position());
    fly->update(time_ms);
  }

  Player::Update(time_ms);
}

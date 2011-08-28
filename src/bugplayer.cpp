// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./bugplayer.h"

#include <vector>

#include "./world.h"
#include "./flower.h"

#define NUM_BUGS 20

using std::vector;

BugPlayer::BugPlayer(CL_DisplayWindow* window, World* world,
                     int width, int height)
  : Player(window, world, width, height) {
  selectedImage = new CL_Sprite(*gc, "Cross2", &world->resources);
  selectedImage->set_alpha(0.3);

  for (int i = 0; i < NUM_BUGS; i++) {
    const char* name;

    if (i < NUM_BUGS/2)
      name = "Bug1";
    else
      name = "Bug2";

    Fly * fly = new Fly(world, *gc, name);
    fly->set_position(CL_Vec2f(i * 60, i * 60));

    AddFly(fly);
  }
}

void BugPlayer::AddFly(Fly* fly) {
  flies.push_back(fly);
  world->addFly(fly);
}

void BugPlayer::SelectButtonPressed() {
  Flower* nearest_flower = world->NearestFlower(position());

  flies[0]->SetTargetFlower(nearest_flower);
}

void BugPlayer::CancelButtonPressed() {
}

void BugPlayer::BuildButtonPressed() {
}

Flower* BugPlayer::GetFreeFlower() {
  vector<Flower*> *flowers = world->NearestFlowers(position());

  int size = flowers->size();
  for (int i=0; i < size; i++) {
    if ((*flowers)[i]->free_space())
      return (*flowers)[i];
  }

  return NULL;
}

void BugPlayer::Draw() {
    Flower* flower = GetFreeFlower();

    if (flower != NULL) {
      CL_Vec2f pos = flower->position() - map_position();
      selectedImage->draw(*gc, pos.x, pos.y);
    }
/*  } else if (state == Selected || state == SelectedBuilding) {
    selectedImage->set_alpha(0.8);

    CL_Vec2f pos = selectedFlower->position() - map_position();
    selectedImage->draw(*gc, pos.x, pos.y);
  }*/

  Player::Draw();
}

void BugPlayer::Update(int time_ms) {
  // Make turrets target mousepos
  std::vector<Fly *>::iterator fly_it;
  for (fly_it = flies.begin(); fly_it != flies.end(); ++fly_it) {
    Fly *fly = (*fly_it);

    fly->set_target_position(position());
    fly->update(time_ms);
  }

  Player::Update(time_ms);
}

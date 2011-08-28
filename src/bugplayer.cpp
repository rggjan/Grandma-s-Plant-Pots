// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./bugplayer.h"

#include <vector>

#include "./world.h"

using std::vector;

BugPlayer::BugPlayer(CL_DisplayWindow* window, World* world,
                         int width, int height)
  : Player(window, world, width, height) {
  selectedImage = new CL_Sprite(*gc, "Cross2", &world->resources);
}

void BugPlayer::SelectButtonPressed() {
}

void BugPlayer::CancelButtonPressed() {
}

void BugPlayer::BuildButtonPressed() {
}

void BugPlayer::Draw() {
  /*if (state == Selecting) {
    Flower* nearest_flower = NearestFlower();

    selectedImage->set_alpha(0.3);
    if (nearest_flower != NULL) {
      CL_Vec2f pos = nearest_flower->position() - map_position();
      selectedImage->draw(*gc, pos.x, pos.y);
    }
  } else if (state == Selected || state == SelectedBuilding) {
    selectedImage->set_alpha(0.8);

    CL_Vec2f pos = selectedFlower->position() - map_position();
    selectedImage->draw(*gc, pos.x, pos.y);
  }*/

  Player::Draw();
}

void BugPlayer::Update(int time_ms) {
  for (unsigned int i = 0; i < flies.size(); i++) {
//    flies[i]->Update(time_ms);
  }

  Player::Update(time_ms);
}

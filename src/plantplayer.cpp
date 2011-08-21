// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./plantplayer.h"

#include <vector>

#include "./flower.h"
#include "./player.h"
#include "./world.h"

using std::vector;

PlantPlayer::PlantPlayer(CL_DisplayWindow* window, World* world,
                         int width, int height)
  : Player(window, world, width, height),
    state(Idle) {
  selectedImage = new CL_Sprite(*gc, "Cross", &world->resources);

  tmpFlower = new Flower(world, gc, 0, 0);
  // tmpFlower->setRed();
}

bool PlantPlayer::BuildPlant() {
  if (energy >= Flower::energy_cost && tmpFlower->CanBuild(x(), y())) {
    Flower *flower = new Flower(world, gc, x(), y());

    flowers.push_back(flower);
    world->addFlower(flower);

    energy -= Flower::energy_cost;
    return true;
  } else {
    // TODO(rggjan): beep
    return false;
  }
}

void PlantPlayer::SelectButtonPressed() {
  switch (state) {
  case Building:
    if (BuildPlant())
      state = Idle;
    break;
  default:
    break;
  }
}

void PlantPlayer::CancelButtonPressed() {
  switch (state) {
  case Building:
    state = Idle;
    break;
  default:
    break;
  }
}

void PlantPlayer::BuildButtonPressed() {
  switch (state) {
  case Idle:
    state = Building;
    break;
  case Building:
    BuildPlant();
    break;
  default:
    break;
  }
}

void PlantPlayer::draw() {
  // TODO(rggjan): infinity
  int best_dist = -1;
  Flower *best_flower = NULL;

  // Get nearest flower
  std::vector<Flower *>::iterator it;
  for (it = flowers.begin(); it != flowers.end(); ++it) {
    int x_diff = (*it)->posX - x();
    int y_diff = (*it)->posY - y();
    int dist_squared = y_diff*y_diff + x_diff*x_diff;

    if (best_flower == NULL || dist_squared < best_dist) {
      best_dist = dist_squared;
      best_flower = (*it);
    }
  }

  selectedImage->set_alpha(0.5);

  if (best_flower != NULL)
    selectedImage->draw(*gc, best_flower->posX - center_x,
                        best_flower->posY - center_y);

  Player::draw();
}

void PlantPlayer::DrawEnergy() {
  switch (state) {
  case Building: {
    CL_Colorf color = CL_Colorf::white;
    if (tmpFlower->energy_cost > energy)
      color = CL_Colorf::red;
    default_font.draw_text(*gc, CL_Pointf(10, 30),
                           cl_format("Energy: %1 (%2)", energy,
                                     tmpFlower->energy_cost), color);
    break;
  }
  default:
    Player::DrawEnergy();
  }
}

void PlantPlayer::draw_cross() {
  switch (state) {
  case Idle:
    Player::draw_cross();
    break;
  case Building:
    if (tmpFlower->CanBuild(x(), y()))
      tmpFlower->DrawGreen(gc, cross_x, cross_y);
    else
      tmpFlower->DrawRed(gc, cross_x, cross_y);
    break;
  default:
    break;
  }
}

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
  //tmpFlower->setRed();
}

void PlantPlayer::BuildButtonPressed() {
  switch (state) {
  case Idle:
    state = Building;
    break;
  case Building:
    state = Idle;
    break;
  default:
    break;
  }

  /*
  if (energy >= Flower::energy_cost) {
    Flower *flower = new Flower(world, gc,
                                cross_x + center_x,
                                cross_y + center_y);

    flowers.push_back(flower);
    world->addFlower(flower);

    energy -= Flower::energy_cost;
  } else {
    // TODO(rggjan): beep
  }*/
}

void PlantPlayer::draw() {
  // TODO(rggjan): infinity
  int best_dist = -1;
  Flower *best_flower = NULL;

  // Get nearest flower
  std::vector<Flower *>::iterator it;
  for (it = flowers.begin(); it != flowers.end(); ++it) {
    int x_diff = (*it)->posX - (center_x + cross_x);
    int y_diff = (*it)->posY - (center_y + cross_y);
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

void PlantPlayer::draw_cross() {
  switch (state) {
  case Idle:
    Player::draw_cross();
    break;
  case Building:
    tmpFlower->drawRed(gc, cross_x, cross_y);      
    break;
  default:
    break;
  }
}

// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./plantplayer.h"

#include <vector>

#include "./flower.h"
#include "./player.h"
#include "./world.h"

using std::vector;

PlantPlayer::PlantPlayer(CL_DisplayWindow* window, World* world,
                         int width, int height)
  : Player(window, world, width, height) {
}

void PlantPlayer::ActionButtonPressed() {
  if (energy >= Flower::energy_cost) {
    Flower *flower = new Flower(world, gc,
                                cross_x + center_x,
                                cross_y + center_y);

    flowers.push_back(flower);
    world->addFlower(flower);

    energy -= Flower::energy_cost;
  } else {
    // TODO(rggjan): beep
  }
};

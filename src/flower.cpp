// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./flower.h"

#include <stdlib.h>
#include <ClanLib/display.h>

#include "./world.h"

Flower::Flower(World *world, float posX, float posY)
  : GameObject(world),
    direction(0, -1) {
  CL_GraphicContext gc = world->get_gc();

  flowerGraphic = new CL_Sprite(gc, "Plant1",
                                  &world->resources);
  flowerGraphic->set_play_loop(true);

  this->posX = posX;
  this->posY = posY;
}

bool Flower::update(int timeElapsed_ms) {
  flowerGraphic->update(timeElapsed_ms);
  return true;
}

void Flower::draw(int x, int y) {
  CL_GraphicContext gc = world->get_gc();

  // Draw tankturret
  flowerGraphic->draw(gc, posX-x, posY-y);
}

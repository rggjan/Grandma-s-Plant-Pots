// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./leaf.h"

#include <stdlib.h>
#include <ClanLib/display.h>

#include "./world.h"

Leaf::Leaf(World *world, float posX, float posY)
  : GameObject(world),
    direction(0, -1) {
  CL_GraphicContext gc = world->get_gc();

  leafGraphic = new CL_Sprite(gc, "Leaf1",
                                  &world->resources);
  leafGraphic->set_play_loop(true);

  this->posX = posX;
  this->posY = posY;
}

bool Leaf::update(int timeElapsed_ms) {
  leafGraphic->update(timeElapsed_ms);
  return true;
}

void Leaf::draw(int x, int y) {
  CL_GraphicContext gc = world->get_gc();

  // Draw tankturret
  leafGraphic->draw(gc, posX-x, posY-y);
}

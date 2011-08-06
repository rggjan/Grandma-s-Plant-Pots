// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./flower.h"

#include <stdlib.h>
#include <ClanLib/display.h>

#include "./world.h"

Flower::Flower(World *world, CL_GraphicContext &gc, float posX, float posY)
  : GameObject(world) {
  spriteImage = new CL_Sprite(gc, "Plant1", &world->resources);
  spriteImage->set_play_loop(true);

  this->posX = posX;
  this->posY = posY;
}

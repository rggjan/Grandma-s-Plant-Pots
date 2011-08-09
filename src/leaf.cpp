// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./leaf.h"

#include <stdlib.h>
#include <ClanLib/display.h>

#include "./world.h"

Leaf::Leaf(World *world, CL_GraphicContext &gc, const CL_StringRef &name,
           float posX, float posY)
  : GameObject(world) {
  spriteImage = new CL_Sprite(gc, name, &world->resources);
  spriteImage->set_play_loop(true);

  this->posX = posX;
  this->posY = posY;
}

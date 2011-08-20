// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./flower.h"

#include <stdlib.h>
#include <ClanLib/display.h>

#include "./world.h"

Flower::Flower(World *world, CL_GraphicContext *gc, float posX, float posY)
  : GameObject(world) {
  spriteImage = new CL_Sprite(*gc, "Plant1", &world->resources);
  spriteImage->set_play_loop(true);

  this->posX = posX;
  this->posY = posY;
}

void Flower::drawRed(CL_GraphicContext *gc, int x, int y) {
  // Draw graphic
  spriteImage->set_color(CL_Color::green);
  spriteImage->set_alpha(0.8);
  spriteImage->set_frame(2);
  spriteImage->draw(*gc, x, y);
}

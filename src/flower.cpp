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

void Flower::DrawRed(CL_GraphicContext *gc, int x, int y) {
  // Draw graphic
  spriteImage->set_color(CL_Color::green);
  spriteImage->set_alpha(0.9);
  spriteImage->set_frame(spriteImage->get_frame_count()-1);
  spriteImage->draw(*gc, x, y);
}

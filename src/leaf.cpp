// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./leaf.h"

#include <stdlib.h>
#include <ClanLib/display.h>

#include "./world.h"

Leaf::Leaf(World *world, CL_GraphicContext *gc, const CL_StringRef &name,
           float posX, float posY)
  : GameObject(world) {
  spriteImage = new CL_Sprite(*gc, name, &world->resources);
  spriteImage->set_play_loop(true);

  this->posX = posX;
  this->posY = posY;
}

void Leaf::DrawRed(CL_GraphicContext *gc, int x, int y) {
  // Draw graphic
  spriteImage->set_color(CL_Color::red);
  spriteImage->set_alpha(0.8);
  spriteImage->set_frame(spriteImage->get_frame_count()-1);
  spriteImage->draw(*gc, x, y);
}

void Leaf::DrawGreen(CL_GraphicContext *gc, int x, int y) {
  // Draw graphic
  spriteImage->set_color(CL_Color::green);
  spriteImage->set_alpha(0.8);
  spriteImage->set_frame(spriteImage->get_frame_count()-1);
  spriteImage->draw(*gc, x, y);
}

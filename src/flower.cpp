// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./flower.h"

#include <stdlib.h>
#include <ClanLib/display.h>

#include "./world.h"

Flower::Flower(World *world, CL_GraphicContext *gc, CL_Vec2f position)
  : GameObject(world, position) {
  spriteImage = new CL_Sprite(*gc, "Plant1", &world->resources);
  spriteImage->set_play_loop(true);
}

void Flower::DrawRed(CL_GraphicContext *gc, CL_Vec2f position) {
  // Draw graphic
  spriteImage->set_color(CL_Color::red);
  spriteImage->set_alpha(0.8);
  spriteImage->set_frame(spriteImage->get_frame_count()-1);
  spriteImage->draw(*gc, position.x, position.y);
}

void Flower::DrawGreen(CL_GraphicContext *gc, CL_Vec2f position) {
  // Draw graphic
  spriteImage->set_color(CL_Color::green);
  spriteImage->set_alpha(0.8);
  spriteImage->set_frame(spriteImage->get_frame_count()-1);
  spriteImage->draw(*gc, position.x, position.y);
}

bool Flower::CanBuild(CL_Vec2f position) {
  return world->CanBuild(position);
}

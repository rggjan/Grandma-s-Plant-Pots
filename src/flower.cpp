// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./flower.h"

#include <stdlib.h>
#include <ClanLib/display.h>

#include "./world.h"

#define TIME_TO_OPEN 6000
#define TIME_TO_FINAL 15000
#define MIN_FLOWER_DISTANCE 50

Flower::Flower(World *world, CL_GraphicContext *gc, CL_Vec2f position)
  : GameObject(world, position),
    state_(kClosed),
    age_(0) {
  spriteImage = new CL_Sprite(*gc, "Plant1", &world->resources);
}

void Flower::DrawRed(CL_GraphicContext *gc, CL_Vec2f position) {
  // Draw graphic
  spriteImage->set_color(CL_Color::red);
  spriteImage->set_alpha(0.8);
  spriteImage->set_frame(2);
  spriteImage->draw(*gc, position.x, position.y);
}

void Flower::DrawGreen(CL_GraphicContext *gc, CL_Vec2f position) {
  // Draw graphic
  spriteImage->set_color(CL_Color::green);
  spriteImage->set_alpha(0.8);
  spriteImage->set_frame(2);
  spriteImage->draw(*gc, position.x, position.y);
}

void Flower::Update(int time_ms) {
  age_ += time_ms;

  if (state_ == kClosed) {
    if (age_ > TIME_TO_OPEN) {
      state_ = kOpen;
      spriteImage->set_frame(1);
    }
  }

  if (state_ == kOpen) {
    if (age_ > TIME_TO_OPEN + TIME_TO_FINAL) {
      state_ = kFinal;
      spriteImage->set_frame(2);
    }
  }
}

bool Flower::CanBuild(CL_Vec2f position) {
  Flower *nearest_flower = world->NearestFlower(position);

  if (nearest_flower &&
      (nearest_flower->position() - position).length() < MIN_FLOWER_DISTANCE)
    return false;
  return world->CanBuild(position);
}

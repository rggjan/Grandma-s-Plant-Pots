// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "plants/plant.h"

#include "plants/plantplayer.h"

Plant::Plant(World *world, CL_GraphicContext *gc,
               CL_Vec2f position, PlantPlayer* player)
  : GameObject(world, position),
    player_(player),
    eating_fly_(NULL),
    co2_collected_per_second_(0) {
}

void Plant::DrawRed(CL_GraphicContext *gc, CL_Vec2f position) {
  // Draw graphic
  spriteImage->set_color(CL_Color::red);
  spriteImage->set_alpha(0.8);
  spriteImage->set_frame(2);
  spriteImage->draw(*gc, position.x, position.y);
}

void Plant::DrawGreen(CL_GraphicContext *gc, CL_Vec2f position) {
  // Draw graphic
  spriteImage->set_color(CL_Color::green);
  spriteImage->set_alpha(0.8);
  spriteImage->set_frame(2);
  spriteImage->draw(*gc, position.x, position.y);
}

void Plant::Update(int time_ms) {
  // Update CO2 production
  player_->co2_ += co2_collected_per_second_ * time_ms / 1000.;
}

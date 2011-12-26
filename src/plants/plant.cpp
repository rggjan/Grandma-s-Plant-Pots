// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "plants/plant.h"

#include "plants/plantplayer.h"

#define ZOMBIE_SECONDS 10

Plant::Plant(World *world, CL_GraphicContext *gc,
             CL_Vec2f position, PlantPlayer* player)
  : GameObject(world, position),
    energy_(100),
    player_(player),
    eating_fly_(NULL),
    co2_collected_per_second_(0),
    sun_collected_per_second_(0) {
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

bool Plant::Update(int time_ms) {
  if (!is_alive()) {
    dead_time_ += time_ms;
    // TODO(rggjan): Leaves!
    return (dead_time_ <= ZOMBIE_SECONDS*1000.);
  }

  // Update CO2 production
  player_->co2_ += co2_collected_per_second_ * time_ms / 1000.;

  // Update sun production
  player_->sun_ += sun_collected_per_second_;

  return true;
}

void Plant::Draw(CL_GraphicContext *gc, CL_Vec2f position) {
  CL_Vec2f pos = position_ - position;

  if (!is_alive()) {
    spriteImage->set_color(CL_Color::brown);

    if (dead_time_ > ZOMBIE_SECONDS)
      ; // TODO(rggjan): delete if no leaves left!

    spriteImage->set_alpha(1 - dead_time_/(ZOMBIE_SECONDS*1000.));

    spriteImage->draw(*gc, pos.x, pos.y);
    return;
  }

  spriteImage->draw(*gc, pos.x, pos.y);

  if (energy_ > 20) {
    CL_Draw::line(*gc, pos.x - energy_ / 3., pos.y - 20, pos.x + energy_ / 3., pos.y - 20, CL_Colorf::green);
    CL_Draw::line(*gc, pos.x - energy_ / 3., pos.y - 19, pos.x + energy_ / 3., pos.y - 19, CL_Colorf::darkgreen);
  } else {
    CL_Draw::line(*gc, pos.x - energy_ / 3., pos.y - 20, pos.x + energy_ / 3., pos.y - 20, CL_Colorf::red);
    CL_Draw::line(*gc, pos.x - energy_ / 3., pos.y - 19, pos.x + energy_ / 3., pos.y - 19, CL_Colorf::darkred);
  }
  CL_Draw::line(*gc, pos.x - energy_ / 3. + 1, pos.y - 18, pos.x + energy_ / 3. + 1, pos.y - 18, CL_Colorf::black);
}

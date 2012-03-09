// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "plants/plant.h"

#include "plants/plantplayer.h"

Plant::Plant(World *world, CL_GraphicContext *gc,
             CL_Vec2f position, CL_StringRef name, PlantPlayer* player)
  : GameObject(world, gc, position, name),
    player_(player),
    eating_bug_(NULL),
    co2_collected_per_second_(0),
    sun_collected_per_second_(0) {
  dead_color_ = CL_Color::brown;
}

void Plant::DrawTmp(CL_GraphicContext *gc, bool green) {
  if (green)
    sprite_.set_color(CL_Color::green);
  else
    sprite_.set_color(CL_Color::red);

  sprite_.set_alpha(0.8);
  sprite_.set_frame(sprite_.get_frame_count() - 1);
  sprite_.draw(*gc, player_->cross_position().x, player_->cross_position().y);
}

bool Plant::Update(int time_ms) {
  if (!GameObject::Update(time_ms))
    return false;

  // Update CO2 production
  player_->co2_ += co2_collected_per_second_ * time_ms / 1000.;

  // Update sun production
  player_->sun_ += sun_collected_per_second_;

  return true;
}

void Plant::Draw(CL_GraphicContext *gc, CL_Vec2f position) {
  CL_Vec2f pos = position_ - position;

  GameObject::Draw(gc, position);

  if (!is_alive())
    return;

  // Draw energy
  if (energy_ > 20) {
    CL_Draw::line(*gc, pos.x - energy_ / 3., pos.y - 20,
                  pos.x + energy_ / 3., pos.y - 20, CL_Colorf::green);
    CL_Draw::line(*gc, pos.x - energy_ / 3., pos.y - 19,
                  pos.x + energy_ / 3., pos.y - 19, CL_Colorf::darkgreen);
  } else {
    CL_Draw::line(*gc, pos.x - energy_ / 3., pos.y - 20,
                  pos.x + energy_ / 3., pos.y - 20, CL_Colorf::red);
    CL_Draw::line(*gc, pos.x - energy_ / 3., pos.y - 19,
                  pos.x + energy_ / 3., pos.y - 19, CL_Colorf::darkred);
  }
  CL_Draw::line(*gc, pos.x - energy_ / 3. + 1, pos.y - 18,
                pos.x + energy_ / 3. + 1, pos.y - 18, CL_Colorf::black);
}

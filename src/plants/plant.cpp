// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "plants/plant.h"

#include "plants/plantplayer.h"

Plant::Plant(World *world, clan::Canvas canvas,
			 clan::Vec2f position, std::string name, PlantPlayer* player)
  : GameObject(world, canvas, position, name),
    player_(player),
    eating_bug_(NULL),
    co2_collected_per_second_(0),
    sun_collected_per_second_(0) {
  dead_color_ = clan::Color::brown;
}

void Plant::DrawTmp(clan::Canvas canvas, float x, float y, float alpha, float size, clan::Color color) {
  sprite_.set_color(color);
  sprite_.set_alpha(alpha);
  sprite_.set_frame(sprite_.get_frame_count() - 1);
  float scale;
  sprite_.get_scale(scale, scale);
  sprite_.set_scale(size*scale, size*scale);
  sprite_.draw(canvas, x, y);
  sprite_.set_scale(scale, scale);

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

void Plant::Draw(clan::Canvas canvas, clan::Vec2f position) {
  clan::Vec2f pos = position_ - position;

  GameObject::Draw(canvas, position);

  if (!is_alive())
    return;

  // Draw energy
  if (energy_ > 20) {
    canvas.draw_line(pos.x - energy_ / 3.f, pos.y - 20,
                     pos.x + energy_ / 3.f, pos.y - 20, clan::Colorf::green);
    canvas.draw_line(pos.x - energy_ / 3.f, pos.y - 19,
                     pos.x + energy_ / 3.f, pos.y - 19, clan::Colorf::darkgreen);
  } else {
    canvas.draw_line(pos.x - energy_ / 3.f, pos.y - 20,
                     pos.x + energy_ / 3.f, pos.y - 20, clan::Colorf::red);
    canvas.draw_line(pos.x - energy_ / 3.f, pos.y - 19,
                     pos.x + energy_ / 3.f, pos.y - 19, clan::Colorf::darkred);
  }
  canvas.draw_line(pos.x - energy_ / 3.f + 1, pos.y - 18,
                   pos.x + energy_ / 3.f + 1, pos.y - 18, clan::Colorf::black);
}

// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./gameobject.h"

#define ZOMBIE_SECONDS 10

void GameObject::Draw(CL_GraphicContext *gc, CL_Vec2f target) {
  CL_Vec2f pos = position_ - target;

  if (!is_alive())
    sprite_.set_alpha(1 - dead_time_ / (ZOMBIE_SECONDS * 1000.));

  sprite_.draw(*gc, pos.x, pos.y);
}

void GameObject::UpdateAnimation(int time_ms) {
  sprite_.update(time_ms);
}

bool GameObject::Update(int time_ms) {
  if (!is_alive()) {
    dead_time_ += time_ms;
    return (dead_time_ <= ZOMBIE_SECONDS * 1000.);
  }

  UpdateAnimation(time_ms);

  return true;
}

double GameObject::DecreaseEnergy(double amount) {
  if (amount > energy_)
    amount = energy_;

  energy_ -= amount;

  if (!is_alive())
    sprite_.set_color(dead_color_);

  return amount;
}

GameObject::GameObject(World* world, CL_GraphicContext *gc,
                       CL_Vec2f position, CL_StringRef name)
  : world_(world),
    energy_(1),
    dead_time_(0),
    position_(position),
    sprite_(*gc, name, &world->resources),
    dead_color_(CL_Color::black) {
}

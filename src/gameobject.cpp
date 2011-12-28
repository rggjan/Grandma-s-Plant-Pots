// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./gameobject.h"

#define ZOMBIE_SECONDS 10

void GameObject::Draw(CL_GraphicContext *gc, CL_Vec2f target) {
  CL_Vec2f pos = position_ - target;

  if (!is_alive())
//    spriteImage->set_color(CL_Color::brown);
    spriteImage->set_alpha(1 - dead_time_ / (ZOMBIE_SECONDS * 1000.));

  spriteImage->draw(*gc, pos.x, pos.y);
}

bool GameObject::Update(int time_ms, bool update_animation) {
  if (!is_alive()) {
    dead_time_ += time_ms;
    return (dead_time_ <= ZOMBIE_SECONDS * 1000.);
  }

  if (update_animation)
    spriteImage->update(time_ms);

  return true;
}

double GameObject::DecreaseEnergy(double amount) {
  if (amount > energy_)
    amount = energy_;

  energy_ -= amount;

  if (!is_alive())
    spriteImage->set_color(dead_color_);

  return amount;
}

GameObject::GameObject(World* world, CL_Vec2f position)
  : position_(position),
    energy_(1),
    world_(world),
    dead_time_(0) {
}

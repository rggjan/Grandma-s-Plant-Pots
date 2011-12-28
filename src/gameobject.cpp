// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./gameobject.h"

void GameObject::Draw(CL_GraphicContext *gc, CL_Vec2f target) {
  CL_Vec2f pos = position_ - target;
  spriteImage->draw(*gc, pos.x, pos.y);
}

bool GameObject::Update(int time_ms) {
  spriteImage->update(time_ms);
  return is_alive();
}

double GameObject::DecreaseEnergy(double amount) {
  if (amount > energy_)
    amount = energy_;

  energy_ -= amount;
  return amount;
}

GameObject::GameObject(World* world, CL_Vec2f position)
  : position_(position),
    energy_(1),
    world_(world),
    dead_time_(0) {
}

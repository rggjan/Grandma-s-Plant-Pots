// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./gameobject.h"

#include <string>

#define ZOMBIE_SECONDS 10

GameObject::GameObject(World* world, clan::Canvas canvas,
                       clan::Vec2f position, std::string name)
  : world_(world),
    energy_(1),
    start_energy_(1),
    dead_time_(0),
    position_(position),
    sprite_(clan::Sprite::resource(canvas, name, world->resources)),
    dead_color_(clan::Color::black) {
}

void GameObject::Draw(clan::Canvas canvas, clan::Vec2f target) {
  clan::Vec2f pos = position_ - target;

  if (!is_alive())
    sprite_.set_alpha(1 - dead_time_ / (ZOMBIE_SECONDS * 1000.0f));
  sprite_.set_color(clan::Color::white);
  sprite_.draw(canvas, pos.x, pos.y);
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

double GameObject::DecreaseEnergy(float amount) {
  if (amount > energy_)
    amount = energy_;

  energy_ -= amount;

  if (!is_alive())
    sprite_.set_color(dead_color_);

  return amount;
}

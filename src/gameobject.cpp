// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./gameobject.h"

void GameObject::Draw(CL_GraphicContext *gc, CL_Vec2f target) {
  CL_Vec2f pos = position_ - target;
  spriteImage->draw(*gc, pos.x, pos.y);
}

void GameObject::Update(int time_ms) {
  spriteImage->update(time_ms);
}

GameObject::GameObject(World* world)
  : world_(world),
    alive_(true),
    dead_time_(0) {
}

GameObject::GameObject(World* world, CL_Vec2f position)
  : position_(position),
    world_(world),
    alive_(true),
    dead_time_(0) {
}

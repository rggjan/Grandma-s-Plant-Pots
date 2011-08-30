// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./gameobject.h"

void GameObject::Draw(CL_GraphicContext *gc, CL_Vec2f target) {
  CL_Vec2f pos = position_ - target;
  spriteImage->draw(*gc, pos.x, pos.y);
}

bool GameObject::update(int timeElapsed_ms) {
  spriteImage->update(timeElapsed_ms);
  return true;
}

GameObject::GameObject(World* world) {
  this->world = world;
}

GameObject::GameObject(World* world, CL_Vec2f position)
  : position_(position) {
  this->world = world;
}

// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./gameobject.h"

#include <ClanLib/display.h>

void GameObject::draw(CL_GraphicContext &gc, int x, int y) {
  // Draw graphic
  spriteImage->draw(gc, posX-x, posY-y);
}

bool GameObject::update(int timeElapsed_ms) {
  spriteImage->update(timeElapsed_ms);
  return true;
}

GameObject::GameObject(World* world) {
  this->world = world;
}

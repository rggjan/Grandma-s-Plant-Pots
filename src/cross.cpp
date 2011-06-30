#include "cross.h"

#include <ClanLib/display.h>

#include "world.h"

Cross::Cross(World *world) : GameObject(world) {
  CL_GraphicContext gc = world->get_gc();

  sprite = new CL_Sprite(gc, "Cross", &world->resources);
}

Cross::~Cross() {
  delete sprite;
}

void Cross::setPos(int x, int y) {
  posX = (float)x;
  posY = (float)y;
}

void Cross::draw(int x, int y) {
  CL_GraphicContext gc = world->get_gc();
  sprite->draw(gc, posX, posY);
}

bool Cross::update(int timeElapsed_ms) {
  sprite->update(timeElapsed_ms);
  return true;
}

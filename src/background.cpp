#include "background.h"

#include <ClanLib/display.h>

#include "world.h"

Background::Background(World *world) : GameObject(world) {
  CL_GraphicContext gc = world->get_gc();

  sprite = new CL_Sprite(gc, "Background", &world->resources);
}

Background::~Background() {
  delete sprite;
}

void Background::setPos(int x, int y) {
  posX = (float)x;
  posY = (float)y;
}

void Background::draw(int x, int y) {
  CL_GraphicContext gc = world->get_gc();
  sprite->draw(gc, posX-x, posY-y);
}

bool Background::update(int timeElapsed_ms) {
  sprite->update(timeElapsed_ms);
  return true;
}

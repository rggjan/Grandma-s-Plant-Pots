#include "building.h"

#include <ClanLib/display.h>

#include "world.h"

Building::Building(BuildingType buiding_type, World *world)
  : GameObject(world) {
  CL_GraphicContext gc = world->get_gc();

  switch(buiding_type) {
  case HELI_PAD:
    sprite = new CL_Sprite(gc, "helipad", &world->resources);
    collisionBuilding = new CL_CollisionOutline("Gfx/helipad.png");
    collisionBuilding->set_alignment(origin_center);
    break;
  }

  exploding = false;
}

Building::~Building() {
  delete sprite;
  delete collisionBuilding;
}

void Building::setPos(int x, int y) {
  posX = (float)x;
  posY = (float)y;
  collisionBuilding->set_translation(posX, posY);
}

void Building::setAngle(float newAngle) {
  angle = newAngle;
  sprite->set_angle(CL_Angle(angle, cl_degrees));
  collisionBuilding->set_angle(CL_Angle(angle, cl_degrees));
}

void Building::draw(int x, int y) {

  CL_GraphicContext gc = world->get_gc();
  sprite->draw(gc, posX-x, posY-y);
}

bool Building::update(int timeElapsed_ms) {
  sprite->update(timeElapsed_ms);
  return true;
}

bool Building::hitCheck(CL_CollisionOutline *outline) {
  return collisionBuilding->collide(*outline);
}

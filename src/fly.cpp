#include "precomp.h"

#include "fly.h"
#include "world.h"

#ifndef WIN32
// For rand()
#include <stdlib.h>
#endif

Fly::Fly(World *world) : GameObject(world), direction(0, -1), target_direction(0, -1) {
  CL_GraphicContext gc = world->get_gc();

  spriteDragonfly = new CL_Sprite(gc, "SpaceShootTurretShooting", &world->resources);
}

void Fly::setPos(int x, int y) {
  posX = destPosX = (float)x;
  posY = destPosY = (float)y;
}

void Fly::setTargetPos(int x, int y) {
  target_direction = CL_Vec2f(x - posX, y - posY);
  target_direction.normalize();
}



bool Fly::update(int timeElapsed_ms) {
  spriteDragonfly->update(timeElapsed_ms);
  spriteDragonfly->set_play_loop(true);

  direction = direction + target_direction/10;
  direction.normalize();

  // Calculate angle from current sprite position to mouse position

  CL_Vec2f up(0.0f, -1.0f);
  float angle = up.angle(direction).to_degrees();
  if(direction.x < 0)
    angle = 360.0f - angle;

  spriteDragonfly->set_angle(CL_Angle(angle, cl_degrees));

  posX += direction.x * 10*(rand()%100)/100;
  posY += direction.y * 10*(rand()%100)/100;

  return true;
}

void Fly::draw(int x, int y) {
  CL_GraphicContext gc = world->get_gc();

  // Draw tankturret
  spriteDragonfly->draw(gc, posX-x, posY-y);
}

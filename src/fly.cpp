#include "precomp.h"

#include "fly.h"
#include "world.h"

#ifndef WIN32
// For rand()
#include <stdlib.h>
#endif

Fly::Fly(World *world) : GameObject(world), direction(0, -1), target_direction(0, -1) {
  CL_GraphicContext gc = world->get_gc();

  spriteTurretShooting = new CL_Sprite(gc, "SpaceShootTurretShooting", &world->resources);

  bodyTurnSpeed = 80.0f;
  turretTurnSpeed = 150.0f;
  moveSpeed = 100.0f;

  bodyAngle = turretAngle = destBodyAngle = deltaBodyAngle = destTurretAngle = deltaTurretAngle = 0.0f;

  selected = false;
  isFiring = false;
  reverse = false;
}

void Fly::setPos(int x, int y) {
  posX = destPosX = (float)x;
  posY = destPosY = (float)y;
}

void Fly::setTurretTargetPos(int x, int y) {
  target_direction = CL_Vec2f(x - posX, y - posY);
  target_direction.normalize();
}

void Fly::setTurretAngle(float angle) {
  turretAngle = angle;
}

bool Fly::update(int timeElapsed_ms) {
  spriteTurretShooting->update(timeElapsed_ms);
  spriteTurretShooting->set_play_loop(true);
  float timeElapsed = (float) timeElapsed_ms / 1000.0f;

  direction = direction + target_direction/10;
  direction.normalize();
  spriteTurretShooting->set_angle(CL_Angle(turretAngle, cl_degrees));

  posX += direction.x * 10*(rand()%100)/100;
  posY += direction.y * 10*(rand()%100)/100;

  return true;
}

void Fly::draw(int x, int y) {
  CL_GraphicContext gc = world->get_gc();

  // Draw tankturret
  spriteTurretShooting->draw(gc, posX-x, posY-y);
}

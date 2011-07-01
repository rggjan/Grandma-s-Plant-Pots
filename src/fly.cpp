#include "precomp.h"

#include "fly.h"
#include "world.h"

#ifndef WIN32
// For rand()
#include <stdlib.h>
#endif

Fly::Fly(World *world) : GameObject(world) {
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
  CL_Vec2f vector(posX - x, posY - y);
  CL_Vec2f up(0.0f, 1.0f);

  // Calculate angle from current sprite position to mouse position
  float angle = up.angle(vector).to_degrees();
  if(x < posX)
    angle = 360.0f - angle;

  setDestTurretAngle(angle);
}

void Fly::setTurretAngle(float angle) {
  turretAngle = angle;
}

void Fly::setDestTurretAngle(float destAngle) {
  destTurretAngle = destAngle;
  deltaTurretAngle = destTurretAngle - turretAngle;

  if(deltaTurretAngle > 180.0f) {
    deltaTurretAngle -= 360.0f;
    turretAngle += 360.0f;
  }
  if(deltaTurretAngle < -180.0f) {
    deltaTurretAngle += 360.0f;
    turretAngle -= 360.0f;
  }
}

bool Fly::update(int timeElapsed_ms) {
  spriteTurretShooting->update(timeElapsed_ms);
  spriteTurretShooting->set_play_loop(true);
  float timeElapsed = (float) timeElapsed_ms / 1000.0f;

  if(deltaTurretAngle) {
    if(deltaTurretAngle > 0.0f) {
      turretAngle += turretTurnSpeed * timeElapsed;
      if(turretAngle > destTurretAngle) {
        turretAngle = destTurretAngle;
        deltaTurretAngle = 0.0f;
      }
    } else {
      turretAngle -= turretTurnSpeed * timeElapsed;
      if(turretAngle < destTurretAngle) {
        turretAngle = destTurretAngle;
        deltaTurretAngle = 0.0f;
      }
    }
  }
  spriteTurretShooting->set_angle(CL_Angle(turretAngle, cl_degrees));

  return true;
}

void Fly::draw(int x, int y) {
  CL_GraphicContext gc = world->get_gc();

  // Draw tankturret
  spriteTurretShooting->draw(gc, posX-x, posY-y);
}

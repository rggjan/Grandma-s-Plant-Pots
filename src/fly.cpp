// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./fly.h"

#include <stdlib.h>
#include <ClanLib/display.h>

#include "./world.h"

#define CONSTANT_ANGLE false
#define SPEED 300
#define MAX_CURVE 5

Fly::Fly(World *world, CL_GraphicContext &gc)
  : GameObject(world),  
    direction(0, -1) {
  spriteImage = new CL_Sprite(gc, "SpaceShootTurretShooting",
                                  &world->resources);
  spriteImage->set_play_loop(true);
}

void Fly::setPos(int x, int y) {
  posX = destPosX = static_cast<float>(x);
  posY = destPosY = static_cast<float>(y);
}

void Fly::setTargetPos(int x, int y) {
  destPosX = x;
  destPosY = y;
}

bool Fly::update(int timeElapsed_ms) {
  GameObject::update(timeElapsed_ms);

  // Calculate target direction
  CL_Vec2f target_direction;
  target_direction = CL_Vec2f(destPosX - posX, destPosY - posY);
  target_direction.normalize();

  // Right angle
  CL_Vec2f right;
  right.x = -direction.y;
  right.y = direction.x;

#if CONSTANT_ANGLE
  if (right.dot(target_direction) > 0)
    direction = direction + right*MAX_CURVE*timeElapsed_ms/1000;
  else
    direction = direction - right*MAX_CURVE*timeElapsed_ms/1000;
#else
  direction = direction + target_direction*MAX_CURVE*timeElapsed_ms/1000;
#endif

  // Normalize direction
  direction.normalize();

  // Update angle
  CL_Vec2f up(0.0f, -1.0f);
  float angle = up.angle(direction).to_degrees();
  if (direction.x < 0)
    angle = 360.0f - angle;

  spriteImage->set_angle(CL_Angle(angle, cl_degrees));

  // Update position
  //posX += direction.x * 10*(rand()%100)/100;
  //posY += direction.y * 10*(rand()%100)/100;
  posX += direction.x*timeElapsed_ms/1000*SPEED;
  posY += direction.y*timeElapsed_ms/1000*SPEED;

  return true;
}

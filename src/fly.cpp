// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./fly.h"

#include <stdlib.h>
#include <ClanLib/display.h>

#include "./world.h"

#define CONSTANT_ANGLE false
#define SPEED 300
#define MAX_CURVE 5
#define ATTACK_SPEED_DECREASE_DISTANCE 200
#define ATTACK_MIN_DISTANCE 5

Fly::Fly(World *world, CL_GraphicContext &gc, const CL_StringRef &name)
  : GameObject(world),
    direction(0, -1),
    target_flower_(NULL) {
  spriteImage = new CL_Sprite(gc, name, &world->resources);

  spriteImage->set_play_loop(true);
}

bool Fly::update(int timeElapsed_ms) {
  GameObject::update(timeElapsed_ms);

  // Calculate target direction
  CL_Vec2f target_direction;

  if (target_flower_ == NULL)
    target_direction = target_position_ - position_;
  else
    target_direction = target_flower_->position() - position_;

  // Calculate distance
  float distance;
  distance = target_direction.length();
  target_direction.normalize();

  if (target_flower_ != NULL and distance < ATTACK_MIN_DISTANCE)
    return true;

  // Right angle
  CL_Vec2f right;
  right.x = -direction.y;
  right.y = direction.x;

#if CONSTANT_ANGLE
  if (right.dot(target_direction) > 0)
    direction = direction + right * MAX_CURVE * timeElapsed_ms / 1000;
  else
    direction = direction - right * MAX_CURVE * timeElapsed_ms / 1000;
#else
  direction = direction + target_direction * MAX_CURVE * timeElapsed_ms / 1000;
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
  // posX += direction.x * 10*(rand()%100)/100;
  // posY += direction.y * 10*(rand()%100)/100;
  float speed;
  if (target_flower_ == NULL) {
    speed = SPEED;
  } else {
    if (distance > ATTACK_SPEED_DECREASE_DISTANCE) {
      speed = SPEED;
    } else {
      CL_Console::write_line("distance: %1", distance);
      
      speed = SPEED*distance/ATTACK_SPEED_DECREASE_DISTANCE;
    }
  }
    position_ += direction * timeElapsed_ms / 1000 * speed;

  return true;
}

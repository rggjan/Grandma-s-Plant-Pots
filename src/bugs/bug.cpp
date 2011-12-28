// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./bug.h"

#include <ClanLib/display.h>

#include "./world.h"
#include "plants/plant.h"
#include "bugs/bugplayer.h"

// Buging
#define CONSTANT_ANGLE false
#define SPEED 300
#define MAX_CURVE 5
#define CURVE_VARIANCE 20

// Attacking
#define ATTACK_SPEED_DECREASE_DISTANCE 200
#define ATTACK_MIN_DISTANCE 5
#define EAT_PER_SECOND 30
#define FOOD_NEEDED_TO_DUPLICATE 45

// General
#define START_ENERGY 30

Bug::Bug(World *world, CL_GraphicContext &gc, const CL_StringRef &name,
         BugPlayer* player)
  // TODO(rggjan): Real position
  : GameObject(world, CL_Vec2f(0, 0)),
    energy_(START_ENERGY),
    direction(0, -1),
    target_plant_(NULL),
    food_eaten_(0),
    bug_name_(name),
    player_(player) {
  spriteImage = new CL_Sprite(gc, name, &world->resources);
  spriteImage->set_play_loop(true);

  double max_add = CURVE_VARIANCE / 100.*MAX_CURVE;
  double diff = (static_cast<double>(rand()) / RAND_MAX) *
                max_add * 2 - max_add;
  curve_ = MAX_CURVE + diff;
}

void Bug::SetTargetPlant(Plant *plant) {
  target_plant_ = plant;
  plant->add_eating_bug(this);
}

void Bug::StopEating() {
  if (this->is_free())
    return;

  target_plant_->remove_flies();
  target_plant_ = NULL;
}

double Bug::DecreaseEnergy(double amount) {
  amount = GameObject::DecreaseEnergy(amount);

  if (!is_alive())
    spriteImage->set_color(CL_Color::red);

  return amount;
}

bool Bug::update(int time_ms) {
  GameObject::Update(time_ms);

  if (!is_alive())
    return true;

  // Check if we can reproduce
  if (food_eaten_ > FOOD_NEEDED_TO_DUPLICATE) {
    food_eaten_ -= FOOD_NEEDED_TO_DUPLICATE;
    player_->CreateBug(bug_name_, position());
  }

  // Calculate target direction
  CL_Vec2f target_direction;

  if (target_plant_ == NULL)
    target_direction = target_position_ - position_;
  else
    target_direction = target_plant_->position() - position_;

  // Calculate distance
  float distance;
  distance = target_direction.length();
  target_direction.normalize();

  if (target_plant_ != NULL and distance < ATTACK_MIN_DISTANCE) {
    double amount = EAT_PER_SECOND * time_ms / 1000.;
    food_eaten_ += target_plant_->DecreaseEnergy(amount);

    if (!target_plant_->is_alive())
      target_plant_ = NULL;

    return true;
  }

  // Right angle
  CL_Vec2f right;
  right.x = -direction.y;
  right.y = direction.x;

#if CONSTANT_ANGLE
  if (right.dot(target_direction) > 0)
    direction = direction + right * curve_ * time_ms / 1000;
  else
    direction = direction - right * curve_ * time_ms / 1000;
#else
  direction = direction + target_direction * curve_ * time_ms / 1000;
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
  if (target_plant_ == NULL) {
    speed = SPEED;
  } else {
    if (distance > ATTACK_SPEED_DECREASE_DISTANCE) {
      speed = SPEED;
    } else {
      speed = SPEED * distance / ATTACK_SPEED_DECREASE_DISTANCE;
    }
  }
  position_ += direction * time_ms / 1000 * speed;

  return true;
}

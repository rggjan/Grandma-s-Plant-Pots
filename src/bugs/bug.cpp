// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./bug.h"

#include <ClanLib/display.h>

#include "./world.h"
#include "plants/plant.h"
#include "bugs/bugplayer.h"

// Buging
#define CONSTANT_ANGLE true
#define SPEED 300
#define MAX_CURVE 5
#define CURVE_VARIANCE 30

// Attacking
#define ATTACK_SPEED_DECREASE_DISTANCE 200
#define ATTACK_MIN_DISTANCE 5
#define EAT_PER_SECOND 1
#define FOOD_NEEDED_TO_DUPLICATE 30

// General
#define START_ENERGY 30
#define START_SIZE 0.2
#define END_SIZE 0.1

Bug::Bug(World *world, clan::Canvas *canvas, clan::Vec2f position,
         const std::string name, BugPlayer* player)
// TODO(rggjan): Real position
  : GameObject(world, canvas, position, name),
    direction(0, -1),
    target_plant_(NULL),
    food_eaten_(0),
    bug_name_(name),
    player_(player) {
  sprite_.set_play_loop(true);

  dead_color_ = clan::Color::red;
  energy_ = START_ENERGY;
  sprite_.set_scale(START_SIZE, START_SIZE);

  // Set curve with variance
  double max_add = CURVE_VARIANCE / 100.*MAX_CURVE;
  double diff = (static_cast<double>(rand()) / RAND_MAX) *
                max_add * 2 - max_add;
  curve_ = MAX_CURVE + diff;

  // Add to world
  world->AddBug(this);
}

Bug::~Bug() {
  world_->RemoveBug(this);
}

double Bug::DecreaseEnergy(double amount) {
  amount = GameObject::DecreaseEnergy(amount);

  if (!is_alive())
    StopEating();

  return amount;
}

void Bug::SetTargetPlant(Plant *plant) {
  target_plant_ = plant;
  plant->add_eating_bug(this);
}

void Bug::StopEating() {
  if (this->is_free())
    return;

  target_plant_->remove_bugs();
  target_plant_ = NULL;
}

bool Bug::Update(int time_ms, clan::Vec2f target_position) {
  target_position_ = target_position;

  GameObject::Update(time_ms);

  if (!is_alive())
    return true;

  float scale = (START_SIZE - END_SIZE)*energy_/START_ENERGY + END_SIZE;
  sprite_.set_scale(scale, scale);

  // Check if we can reproduce
  if (food_eaten_ > FOOD_NEEDED_TO_DUPLICATE) {
    food_eaten_ -= FOOD_NEEDED_TO_DUPLICATE;
    player_->CreateBug(bug_name_, position());
  }

  // Calculate target direction
  clan::Vec2f target_direction;

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
  clan::Vec2f right;
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
  clan::Vec2f up(0.0f, -1.0f);
  float angle = up.angle(direction).to_degrees();
  if (direction.x < 0)
    angle = 360.0f - angle;

  sprite_.set_angle(clan::Angle(angle, clan::degrees));

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

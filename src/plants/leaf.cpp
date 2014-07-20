// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./leaf.h"

#include "plants/flower.h"
#include "./plantplayer.h"

#define MIN_LEAF_DISTANCE 50
#define LEAF_MAX_DISTANCE 80

#define CO2_COLLECTED_PER_SECOND 0.001
#define SUN_COLLECTED_PER_SECOND 0.02
#define START_ENERGY 30

Leaf::Leaf(World *world, clan::Canvas canvas,
           clan::Vec2f position, Flower* flower, bool menu)
  : Plant(world, canvas, position, "Leaf2", flower->player(), menu),
    flower_(flower) {
  co2_collected_per_second_ = CO2_COLLECTED_PER_SECOND;
  sun_collected_per_second_ = SUN_COLLECTED_PER_SECOND;
  energy_ = start_energy_ = START_ENERGY;
  sprite_.set_frame(sprite_.get_frame_count() - 1);
}

Leaf::~Leaf() {
  world_->RemovePlant(this);
}

Plant *Leaf::GetNewPlant(clan::Vec2f position, clan::Canvas canvas)
{
  Leaf *new_leaf = new Leaf(world_, canvas, position, flower_, false);
  new_leaf->set_angle(angle());
  flower_->AddLeaf(new_leaf);
  return new_leaf;
}

double Leaf::DecreaseEnergy(float amount)
{
  amount = GameObject::DecreaseEnergy(amount);

  sprite_.set_frame(energy_ / start_energy_ * sprite_.get_frame_count());

  return amount;
}

void Leaf::Draw(clan::Canvas canvas, clan::Vec2f position)
{
  GameObject::Draw(canvas, position);
}

bool Leaf::CanBuild(clan::Vec2f position) {
  if (!flower_->open())
    return false;

  Leaf* nearest_leaf = flower_->NearestLeaf(position);

  if (nearest_leaf &&
      (nearest_leaf->position() - position).length() < MIN_LEAF_DISTANCE)
    return false;

  if (!player_->CanBuild(this))
    return false;

  clan::Vec2f diff = position - flower_->position();
  if (diff.length() > LEAF_MAX_DISTANCE)
     return false;

  return true;
}

// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./leaf.h"

#include "plants/flower.h"

#define MIN_LEAF_DISTANCE 50
#define CO2_COLLECTED_PER_SECOND 0.001
#define SUN_COLLECTED_PER_SECOND 0.02
#define START_ENERGY 30

Leaf::Leaf(World *world, clan::Canvas canvas,
           clan::Vec2f position, Flower* flower, bool menu)
  : Plant(world, canvas, position, "Leaf2", flower->player(), menu),
    flower_(flower) {
  co2_collected_per_second_ = CO2_COLLECTED_PER_SECOND;
  sun_collected_per_second_ = SUN_COLLECTED_PER_SECOND;
  energy_ = START_ENERGY;
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

bool Leaf::CanBuild(clan::Vec2f position, Flower* flower) {
  if (!flower->open())
    return false;

  Leaf* nearest_leaf = flower->NearestLeaf(position);

  if (nearest_leaf &&
      (nearest_leaf->position() - position).length() < MIN_LEAF_DISTANCE)
    return false;

  return true;
}

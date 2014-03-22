// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./leaf.h"

#include "plants/flower.h"

#define MIN_LEAF_DISTANCE 50
#define CO2_COLLECTED_PER_SECOND 0.001
#define SUN_COLLECTED_PER_SECOND 0.02
#define START_ENERGY 30

Leaf::Leaf(World *world, clan::Canvas *canvas,
           clan::Vec2f position, const std::string &name, Flower* flower)
  : Plant(world, canvas, position, name, flower->player()),
    flower_(flower) {
  co2_collected_per_second_ = CO2_COLLECTED_PER_SECOND;
  sun_collected_per_second_ = SUN_COLLECTED_PER_SECOND;
  energy_ = START_ENERGY;

  world_->AddPlant(this);
}

Leaf::~Leaf() {
  world_->RemovePlant(this);
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

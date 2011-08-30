// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./leaf.h"

#include "plants/flower.h"

#define MIN_LEAF_DISTANCE 30
#define CO2_COLLECTED_PER_SECOND 0.01

Leaf::Leaf(World *world, CL_GraphicContext *gc, const CL_StringRef &name,
           CL_Vec2f position, Flower* flower)
  : Plant(world, gc, position, flower->player()),
    flower_(flower) {
  spriteImage = new CL_Sprite(*gc, name, &world->resources);

  co2_collected_per_second_ = CO2_COLLECTED_PER_SECOND;
}

bool Leaf::CanBuild(CL_Vec2f position, Flower* flower) {
  if (flower->state() < kOpen) {
    return false;
  }

  Leaf* nearest_leaf = flower->NearestLeaf(position);

  if (nearest_leaf &&
      (nearest_leaf->position() - position).length() < MIN_LEAF_DISTANCE)
    return false;

  return true;
}

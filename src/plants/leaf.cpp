// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./leaf.h"

#include "plants/flower.h"

#define ENERGY_COLLECTED_PER_SECOND 0.1
#define MIN_LEAF_DISTANCE 30

Leaf::Leaf(World *world, CL_GraphicContext *gc, const CL_StringRef &name,
           CL_Vec2f position, Flower* flower)
  : Plant(world, gc, position, flower->player()),
    flower_(flower) {
  spriteImage = new CL_Sprite(*gc, name, &world->resources);
}
float Leaf::Update(int time_ms) {
  return ENERGY_COLLECTED_PER_SECOND * time_ms / 1000;
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

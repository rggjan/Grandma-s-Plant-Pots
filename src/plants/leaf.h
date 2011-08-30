// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_PLANTS_LEAF_H_
#define SRC_PLANTS_LEAF_H_

#include "./plant.h"

#define LEAF_ENERGY_COST 5

class Flower;

class Leaf : public Plant {
    // Construction
  public:
    Leaf(World *world, CL_GraphicContext *gc, const CL_StringRef &name,
         CL_Vec2f position, Flower *flower);

    static const int energy_cost = LEAF_ENERGY_COST;

    bool CanBuild(CL_Vec2f position, Flower* flower);

    float Update(int time_ms);

    Flower* flower_;
};

#endif  // SRC_PLANTS_LEAF_H_

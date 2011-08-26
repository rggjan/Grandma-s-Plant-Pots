// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_LEAF_H_
#define SRC_LEAF_H_

#include <ClanLib/core.h>

#include "./gameobject.h"

#define ENERGY_COST 10
#define ENERGY_COLLECTED_PER_SECOND 0.1

class Flower;

class Leaf : public GameObject {
    // Construction
  public:
    Leaf(World *world, CL_GraphicContext *gc, const CL_StringRef &name,
         CL_Vec2f position, Flower *flower);

    void DrawRed(CL_GraphicContext *gc, CL_Vec2f position);
    void DrawGreen(CL_GraphicContext *gc, CL_Vec2f position);

    static const int energy_cost = ENERGY_COST;

    bool CanBuild(CL_Vec2f position, Flower* flower);

    void SetAngle(CL_Angle angle);

    Flower* flower_;
};

#endif  // SRC_LEAF_H_

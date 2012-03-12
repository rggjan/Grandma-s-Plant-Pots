// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_PLANTS_LEAF_H_
#define SRC_PLANTS_LEAF_H_

#include "./plant.h"

#define LEAF_SUGAR_COST 3

class Flower;

class Leaf : public Plant {
    // Construction
  public:
    Leaf(World *world, CL_GraphicContext *gc,
           CL_Vec2f position, const CL_StringRef &name, Flower* flower);

    ~Leaf();

    virtual int sugar_cost() { return LEAF_SUGAR_COST; }
    virtual Plant *GetNewPlant(CL_Vec2f position, CL_GraphicContext *gc) {return NULL;}

    bool CanBuild(CL_Vec2f position, Flower* flower);

    Flower* flower_;
};

#endif  // SRC_PLANTS_LEAF_H_

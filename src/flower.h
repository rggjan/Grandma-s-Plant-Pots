// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_FLOWER_H_
#define SRC_FLOWER_H_

#include <ClanLib/core.h>

#include "./gameobject.h"

#define ENERGY_COST 10

class Flower : public GameObject {
  // Construction
  public:
    Flower(World *world, CL_GraphicContext *gc, float posX, float posY);

    void DrawRed(CL_GraphicContext *gc, int x, int y);    

    static const int energy_cost = ENERGY_COST;
};

#endif  // SRC_FLOWER_H_

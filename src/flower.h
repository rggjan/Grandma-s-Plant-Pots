// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_FLOWER_H_
#define SRC_FLOWER_H_

#include <ClanLib/core.h>

#include "./gameobject.h"

#define ENERGY_COST 10

enum FlowerState {
  kClosed,
  kOpen,
  kFinal
};

class Flower : public GameObject {
  // Construction
  public:
  Flower(World *world, CL_GraphicContext *gc, CL_Vec2f position);

  void DrawRed(CL_GraphicContext *gc, CL_Vec2f position);
  void DrawGreen(CL_GraphicContext *gc, CL_Vec2f position);
  bool CanBuild(CL_Vec2f position);

  void Update(int time_ms);

  static const int energy_cost = ENERGY_COST;

  FlowerState state_;
};

#endif  // SRC_FLOWER_H_

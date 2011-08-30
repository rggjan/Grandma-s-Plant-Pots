// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_PLANTS_FLOWER_H_
#define SRC_PLANTS_FLOWER_H_

#include <vector>

#include "./plant.h"

#define ENERGY_COST 10

class Leaf;

enum FlowerState {
  kClosed = 0,
  kOpen = 1,
  kFinal = 2
};

class Flower : public Plant {
  public:
    // Static
    static const int energy_cost = ENERGY_COST;

    // Construction
    Flower(World *world, CL_GraphicContext *gc,
           CL_Vec2f position, PlantPlayer* player);

    // Operations
    void AddLeaf(Leaf* leaf);
    void Update(int time_ms);
    void Draw(CL_GraphicContext* gc, CL_Vec2f target);

    // Queries
    bool CanBuild(CL_Vec2f position);
    Leaf* NearestLeaf(CL_Vec2f position);

    inline FlowerState state() {
      return state_;
    };

  private:
    FlowerState state_;
    std::vector<Leaf*> leaves;
    int age_;
};

#endif  // SRC_PLANTS_FLOWER_H_

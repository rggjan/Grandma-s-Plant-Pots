// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_PLANTS_LEAF_H_
#define SRC_PLANTS_LEAF_H_

#include "./plant.h"

#define LEAF_SUGAR_COST 3

class Flower;

class Leaf : public Plant {

  // Construction
  public:
    Leaf(World *world, clan::Canvas canvas,
         clan::Vec2f position, const std::string &name, Flower* flower);
    ~Leaf();

    virtual Plant *GetNewPlant(clan::Vec2f position, clan::Canvas canvas);

    // Queries
    virtual int sugar_cost() { return LEAF_SUGAR_COST; }
    virtual bool is_master_plant() { return false; }

    bool CanBuild(clan::Vec2f position, Flower* flower);

    Flower* flower_;
};

#endif  // SRC_PLANTS_LEAF_H_

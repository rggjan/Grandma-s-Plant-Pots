// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_PLANTS_FLOWER_H_
#define SRC_PLANTS_FLOWER_H_

#include <list>

#include <ClanLib/sound.h>

#include "./plant.h"

#define ENERGY_COST 6

class Leaf;

enum FlowerState {
  kClosed,
  kOpen,
  kProducing,
  kShooting
};

class Flower : public Plant {
  public:
    // Static
    static const int kSugarCost = ENERGY_COST;

    // Construction
    Flower(World *world, CL_GraphicContext *gc,
           CL_Vec2f position, PlantPlayer* player);

    // Operations
    void AddLeaf(Leaf* leaf);
    bool Update(int time_ms);
    void Draw(CL_GraphicContext* gc, CL_Vec2f target);

    // Queries
    bool CanBuild(CL_Vec2f position);
    Leaf* NearestLeaf(CL_Vec2f position);

    inline FlowerState state() {
      return state_;
    };

  private:
    FlowerState state_;
    std::list<Leaf*> leaves;
    int age_;

    CL_SoundBuffer sound_shot_;
    CL_SoundBuffer_Session sound_session_shot_;
    Bug *targeting_bug;
};

#endif  // SRC_PLANTS_FLOWER_H_

// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_PLANTS_PLANT_H_
#define SRC_PLANTS_PLANT_H_

#include "./gameobject.h"
#include "./world.h"

class PlantPlayer;

class Plant : public GameObject {
  public:
    // Construction
    Plant(World *world, CL_GraphicContext *gc,
          CL_Vec2f position, PlantPlayer* player);

    // Operations
    void DrawRed(CL_GraphicContext *gc, CL_Vec2f position);
    void DrawGreen(CL_GraphicContext *gc, CL_Vec2f position);

    // Queries
    virtual inline bool CanBuild(CL_Vec2f position) {
      return world->CanBuild(position);
    }

    bool free_space() {
      return eating_fly_ == NULL;
    }

    void add_eating_fly(Fly* fly) {
      eating_fly_ = fly;
    }

    void remove_flies() {
      eating_fly_ = NULL;
    }

    PlantPlayer* player() { return player_; }

  protected:
    PlantPlayer* player_;
    Fly* eating_fly_;
};

#endif  // SRC_PLANTS_PLANT_H_

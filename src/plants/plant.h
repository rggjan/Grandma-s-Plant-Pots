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

    // Attributes
    double energy_;

    // Operations
    void DrawRed(CL_GraphicContext *gc, CL_Vec2f position);
    void DrawGreen(CL_GraphicContext *gc, CL_Vec2f position);
    void Draw(CL_GraphicContext *gc, CL_Vec2f position);

    // Queries
    virtual inline bool CanBuild(CL_Vec2f position) {
      return world_->CanBuild(position);
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

    PlantPlayer* player() {
      return player_;
    }

    virtual bool Update(int time_ms);

  protected:
    PlantPlayer* player_;
    Fly* eating_fly_;

    double co2_collected_per_second_;
    double sun_collected_per_second_;
};

#endif  // SRC_PLANTS_PLANT_H_

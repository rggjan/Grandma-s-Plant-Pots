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
          CL_Vec2f position, CL_StringRef name, PlantPlayer* player);

    virtual inline int sugar_cost() = 0;

    virtual Plant *GetNewPlant(CL_Vec2f position, CL_GraphicContext *gc) = 0;

    // Operations
    virtual bool has_children() { return false; }
    virtual void DrawTmp(CL_GraphicContext *gc);
    virtual void DrawTmpChild(CL_GraphicContext *gc) {};
    virtual void Draw(CL_GraphicContext *gc, CL_Vec2f position);

    // Queries
    virtual inline bool CanBuild(CL_Vec2f position) {
      return world_->CanBuild(position);
    }

    bool free_space() {
      return eating_bug_ == NULL;
    }

    void add_eating_bug(Bug* bug) {
      eating_bug_ = bug;
    }

    void remove_bugs() {
      eating_bug_ = NULL;
    }

    PlantPlayer* player() {
      return player_;
    }

    virtual bool Update(int time_ms);

    virtual void UpdateAnimation(int time_ms) {}

  protected:
    PlantPlayer* player_;
    Bug* eating_bug_;

    double co2_collected_per_second_;
    double sun_collected_per_second_;
};

#endif  // SRC_PLANTS_PLANT_H_

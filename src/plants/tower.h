// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_PLANTS_TOWER_H_
#define SRC_PLANTS_TOWER_H_

#include "./tower.h"

#include <ClanLib/sound.h>

#include "./plant.h"

#define ENERGY_COST 6

class Tower : public Plant {
  public:
    // Static
    virtual inline int sugar_cost() { return ENERGY_COST; }

    // Construction
    Tower(World *world, CL_GraphicContext *gc,
           CL_Vec2f position, PlantPlayer* player);

    virtual Plant *GetNewPlant(CL_Vec2f position, CL_GraphicContext *gc);
           
    ~Tower();

    // Operations
    bool Update(int time_ms);
    void Draw(CL_GraphicContext* gc, CL_Vec2f target);

    // Queries
    bool CanBuild(CL_Vec2f position);

    inline bool open() {
      return open_;
    };

  private:
    bool open_;
    int age_;

    CL_SoundBuffer sound_shot_;
    static CL_SoundBuffer_Session sound_session_shot_;
    Bug *targeting_bug;
};

#endif  // SRC_PLANTS_TOWER_H_

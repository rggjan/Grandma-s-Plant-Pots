// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_PLANTS_TOWER_H_
#define SRC_PLANTS_TOWER_H_

#include <string>

#include <ClanLib/sound.h>

#include "./masterplant.h"

class Tower : public MasterPlant {
  public:
    // Static
    virtual inline int sugar_cost() { return 8; }

    // Construction
    Tower(World *world, clan::Canvas canvas,
           clan::Vec2f position, PlantPlayer* player, bool menu=false);

    virtual Plant *GetNewPlant(clan::Vec2f position, clan::Canvas canvas);
           
    ~Tower();

    // Operations
    bool Update(int time_ms);
    void Draw(clan::Canvas canvas, clan::Vec2f target);

    // Queries
    inline bool open() {
      return open_;
    }

  private:
    bool open_;
    int age_;

    clan::SoundBuffer sound_shot_;
    static clan::SoundBuffer_Session sound_session_shot_;
    Bug *targeting_bug;
};

#endif  // SRC_PLANTS_TOWER_H_

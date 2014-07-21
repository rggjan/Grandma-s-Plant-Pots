// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_PLANTS_CACTUS_H_
#define SRC_PLANTS_CACTUS_H_

#include <string>

#include <ClanLib/sound.h>

#include <list>

#include "./masterplant.h"

#define ENERGY_COST 24

class Leaf;

class Cactus : public MasterPlant {
  public:
    // Static
    virtual inline int sugar_cost() { return ENERGY_COST; }

    // Construction
    Cactus(World *world, clan::Canvas canvas,
           clan::Vec2f position, PlantPlayer* player, bool menu = false);

    virtual Plant *GetNewPlant(clan::Vec2f position, clan::Canvas canvas);
    void DrawTmpChild(clan::Canvas *canvas);

    ~Cactus();

    // Operations

    bool Update(int time_ms);





    bool open() { return open_; }

  private:

    int age_;
    bool open_;

    clan::SoundBuffer sound_shot_;
    static clan::SoundBuffer_Session sound_session_shot_;
    Bug *targeting_bug;

};

#endif  // SRC_PLANTS_CACTUS_H_

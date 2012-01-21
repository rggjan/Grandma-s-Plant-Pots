// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./tower.h"

#include <vector>
#include <list>
#include <algorithm>

#include "bugs/bug.h"
#include "plants/plantplayer.h"

#define TIME_TO_OPEN 150
#define MIN_MASTER_PLANT_DISTANCE 100

#define START_ENERGY 30

#define ATTACK_DISTANCE 100
#define ATTACK_ENERGY_PER_SECOND 10

CL_SoundBuffer_Session Tower::sound_session_shot_;

Tower::Tower(World *world, CL_GraphicContext *gc,
             CL_Vec2f position, PlantPlayer* player)
  : Plant(world, gc, position, "Tower", player),
    open_(false),
    age_(0),
    sound_shot_("TowerShoot", &world->resources) {
  energy_ = START_ENERGY;

  sound_shot_.set_volume(0.5f);
  sound_session_shot_ = sound_shot_.prepare();

  world_->AddMasterPlant(this);
}

Tower::~Tower() {
  world_->RemoveMasterPlant(this);
}

bool Tower::Update(int time_ms) {
  if (!is_alive()) {
    return Plant::Update(time_ms);
  }

  // Update state
  age_ += time_ms;

  if (!open() && age_ > TIME_TO_OPEN) {
    open_ = true;
    sprite_.set_frame(1);
  }

  if (open()) {
    std::list<Bug*> *bugs = world_->NearestBugs(position());

    targeting_bug = NULL;

for (Bug * bug : *bugs) {
      if ((position() - bug->position()).length() <= ATTACK_DISTANCE &&
          bug->is_alive()) {
        targeting_bug = bug;
        break;
      }
    }

    if (targeting_bug)
      targeting_bug->DecreaseEnergy(ATTACK_ENERGY_PER_SECOND * time_ms / 1000.);

    return true;
  }

  return Plant::Update(time_ms);
}

bool Tower::CanBuild(CL_Vec2f position) {
  Plant *nearest_plant = world_->NearestMasterPlant(position);

  if (nearest_plant && (nearest_plant->position() - position).length()
      < MIN_MASTER_PLANT_DISTANCE)
    return false;

  return world_->CanBuild(position);
}

void Tower::Draw(CL_GraphicContext* gc, CL_Vec2f target) {
  if (!is_alive()) {
    Plant::Draw(gc, target);
    return;
  }

  // Shoot!
  if (open() && targeting_bug) {
    CL_Draw::line(*gc, position() - target,
                  targeting_bug->position() - target,
                  CL_Colorf::green);
    if (!sound_session_shot_.is_playing())
      sound_session_shot_ = sound_shot_.play();
  }

  Plant::Draw(gc, target);
}

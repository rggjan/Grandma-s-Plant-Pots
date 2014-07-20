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

clan::SoundBuffer_Session Tower::sound_session_shot_;

Tower::Tower(World *world, clan::Canvas canvas,
             clan::Vec2f position, PlantPlayer* player, bool menu)
  : Plant(world, canvas, position, "Tower", player),
    open_(false),
    age_(0),
    sound_shot_(clan::SoundBuffer::resource("TowerShoot", world->resources)) {
  energy_ = START_ENERGY;

  sound_shot_.set_volume(0.5f);
  sound_session_shot_ = sound_shot_.prepare();

  if (!menu)
    world_->AddMasterPlant(this);
}

Plant* Tower::GetNewPlant(clan::Vec2f position, clan::Canvas canvas) {
  return new Tower(world_, canvas, position, player_);
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

    std::list<Bug*>::iterator bug;
    for (bug = bugs->begin(); bug != bugs->end(); ++bug) {
      if ((position() - (*bug)->position()).length() <= ATTACK_DISTANCE &&
          (*bug)->is_alive()) {
        targeting_bug = *bug;
        break;
      }
    }

    if (targeting_bug)
      targeting_bug->DecreaseEnergy(ATTACK_ENERGY_PER_SECOND * time_ms / 1000.);

    return true;
  }

  return Plant::Update(time_ms);
}

bool Tower::CanBuild(clan::Vec2f position) {
  Plant *nearest_plant = world_->NearestMasterPlant(position);

  if (nearest_plant && (nearest_plant->position() - position).length()
      < MIN_MASTER_PLANT_DISTANCE)
    return false;

  if (!player_->CanBuild(this))
    return false;

  return world_->CanBuild(position);
}

void Tower::Draw(clan::Canvas canvas, clan::Vec2f target) {
  if (!is_alive()) {
    Plant::Draw(canvas, target);
    return;
  }

  // Shoot!
  if (open() && targeting_bug) {
      canvas.draw_line(position() - target,
                  targeting_bug->position() - target,
                  clan::Colorf::green);
    if (!sound_session_shot_.is_playing())
      sound_session_shot_ = sound_shot_.play();
  }

  Plant::Draw(canvas, target);
}

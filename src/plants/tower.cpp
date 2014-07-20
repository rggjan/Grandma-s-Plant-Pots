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
  : MasterPlant(world, canvas, position, "Tower", player, menu),
    open_(false),
    age_(0),
    sound_shot_(clan::SoundBuffer::resource("TowerShoot", world->resources)) {
  energy_ = START_ENERGY;

  sound_shot_.set_volume(0.5f);
  sound_session_shot_ = sound_shot_.prepare();
}

Plant* Tower::GetNewPlant(clan::Vec2f position, clan::Canvas canvas) {
  return new Tower(world_, canvas, position, player_);
}

Tower::~Tower() {
  world_->RemovePlant(this);
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

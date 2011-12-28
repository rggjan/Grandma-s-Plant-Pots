// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./flower.h"

#include <vector>
#include <list>

#include "bugs/bug.h"
#include "./leaf.h"
#include "plants/plantplayer.h"

#define TIME_TO_OPEN 1500
#define TIME_TO_FINAL 3000
#define MIN_FLOWER_DISTANCE 50

#define CO2_COLLECTED_PER_SECOND 0.1
#define SUN_COLLECTED_PER_SECOND 0.01
#define START_ENERGY 100

#define ATTACK_DISTANCE 100
#define ATTACK_ENERGY_PER_SECOND 0.5

CL_SoundBuffer_Session Flower::sound_session_shot_;

Flower::Flower(World *world, CL_GraphicContext *gc,
               CL_Vec2f position, PlantPlayer* player)
  : Plant(world, gc, position, player),
    state_(kClosed),
    age_(0),
    sound_shot_("FlowerShoot", &world->resources) {
  spriteImage = new CL_Sprite(*gc, "Plant1", &world->resources);
  co2_collected_per_second_ = CO2_COLLECTED_PER_SECOND;
  sun_collected_per_second_ = SUN_COLLECTED_PER_SECOND;
  energy_ = START_ENERGY;

  sound_shot_.set_volume(0.5f);
  sound_session_shot_ = sound_shot_.prepare();
}

void Flower::AddLeaf(Leaf* leaf) {
  leaves.push_back(leaf);
  state_ = kProducing;
}

bool Flower::Update(int time_ms) {
  // Update leaves
  std::list<Leaf *>::iterator it;
  for (it = leaves.begin(); it != leaves.end();) {
    if (!(*it)->Update(time_ms)) {
      delete *it;
      it = leaves.erase(it);
    } else {
      ++it;
    }
  }

  if (!is_alive()) {
    return Plant::Update(time_ms) || leaves.size() > 0;
  }

  // Update state
  age_ += time_ms;

  if (state_ == kClosed) {
    if (age_ > TIME_TO_OPEN) {
      state_ = kOpen;
      spriteImage->set_frame(1);
    }
  }

  if (state_ == kOpen) {
    if (age_ > TIME_TO_OPEN + TIME_TO_FINAL) {
      state_ = kShooting;
      spriteImage->set_frame(2);
    }
  }

  if (state_ == kShooting) {
    std::vector<Bug*> *bugs = world_->NearestBugs(position());

    targeting_bug = NULL;

    int size = bugs->size();
    for (int i = 0; i < size; i++) {
      Bug* bug = (*bugs)[i];

      if ((position() - bug->position()).length() <= ATTACK_DISTANCE &&
          bug->is_alive()) {
        if (bug->energy_ > 0) {
          targeting_bug = bug;
          break;
        }
      } else {
        break;
      }
    }

    if (targeting_bug) {
      targeting_bug->DecreaseEnergy(ATTACK_ENERGY_PER_SECOND * time_ms / 100);
      if (!targeting_bug->is_alive())
        targeting_bug->StopEating();
    }
  }

  return Plant::Update(time_ms);
}

Leaf* Flower::NearestLeaf(CL_Vec2f position) {
  // TODO(rggjan): infinity
  int best_dist = -1;
  Leaf *nearest_leaf = NULL;

  // Get nearest flower
  std::list<Leaf *>::iterator it;
  for (it = leaves.begin(); it != leaves.end(); ++it) {
    if (!(*it)->is_alive())
      continue;

    float distance = ((*it)->position() - position).length();

    if (nearest_leaf == NULL || distance < best_dist) {
      best_dist = distance;
      nearest_leaf = (*it);
    }
  }

  return nearest_leaf;
}

bool Flower::CanBuild(CL_Vec2f position) {
  Flower *nearest_flower = world_->NearestFlower(position);

  if (nearest_flower &&
      (nearest_flower->position() - position).length() < MIN_FLOWER_DISTANCE)
    return false;

  return world_->CanBuild(position);
}

void Flower::Draw(CL_GraphicContext* gc, CL_Vec2f target) {
  std::list<Leaf *>::iterator it;
  for (it = leaves.begin(); it != leaves.end(); ++it) {
    (*it)->Draw(gc, target);
    /*CL_Draw::line(*gc, position() - player_->map_position(),
                  leaves[i]->position() - player_->map_position(),
                  CL_Colorf::green);*/
  }

  if (!is_alive()) {
    Plant::Draw(gc, target);
    return;
  }

  // Shoot!
  if (state_ == kShooting && targeting_bug) {
    CL_Draw::line(*gc, position() - target,
                  targeting_bug->position() - target,
                  CL_Colorf::green);
    if (!sound_session_shot_.is_playing())
      sound_session_shot_ = sound_shot_.play();
  }

  Plant::Draw(gc, target);
}

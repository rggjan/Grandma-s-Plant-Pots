// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./flower.h"
#include "bugs/fly.h"

#include <vector>

#include "./leaf.h"
#include "plants/plantplayer.h"

#define TIME_TO_OPEN 15000
#define TIME_TO_FINAL 30000
#define MIN_FLOWER_DISTANCE 50

#define CO2_COLLECTED_PER_SECOND 0.1
#define SUN_COLLECTED_PER_SECOND 0.01
#define START_ENERGY 100

#define ATTACK_ENERGY_PER_SECOND 0.5

Flower::Flower(World *world, CL_GraphicContext *gc,
               CL_Vec2f position, PlantPlayer* player)
  : Plant(world, gc, position, player),
    state_(kClosed),
    age_(0) {
  spriteImage = new CL_Sprite(*gc, "Plant1", &world->resources);
  co2_collected_per_second_ = CO2_COLLECTED_PER_SECOND;
  sun_collected_per_second_ = SUN_COLLECTED_PER_SECOND;
  energy_ = START_ENERGY;
}

void Flower::AddLeaf(Leaf* leaf) {
  leaves.push_back(leaf);
}

void Flower::Update(int time_ms) {
  // Update leaves
  unsigned int size = leaves.size();
  for (unsigned int i = 0; i < size; i++) {
    leaves[i]->Update(time_ms);
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
    std::vector<Fly*> *bugs = world->NearestBugs(position());

    targeting_fly = NULL;

    int size = bugs->size();
    for (int i = 0; i < size; i++) {
      Fly* bug = (*bugs)[i];

      if (bug->energy_ > 0) {
        targeting_fly = bug;
        break;
      }
    }

    if(targeting_fly) {
      targeting_fly->energy_ -= ATTACK_ENERGY_PER_SECOND*time_ms/100;
    }
  }

  Plant::Update(time_ms);
}

Leaf* Flower::NearestLeaf(CL_Vec2f position) {
  // TODO(rggjan): infinity
  int best_dist = -1;
  Leaf *nearest_leaf = NULL;

  // Get nearest flower
  std::vector<Leaf *>::iterator it;
  for (it = leaves.begin(); it != leaves.end(); ++it) {
    float distance = ((*it)->position() - position).length();

    if (nearest_leaf == NULL || distance < best_dist) {
      best_dist = distance;
      nearest_leaf = (*it);
    }
  }

  return nearest_leaf;
}


bool Flower::CanBuild(CL_Vec2f position) {
  Flower *nearest_flower = world->NearestFlower(position);

  if (nearest_flower &&
      (nearest_flower->position() - position).length() < MIN_FLOWER_DISTANCE)
    return false;

  return world->CanBuild(position);
}

void Flower::Draw(CL_GraphicContext* gc, CL_Vec2f target) {
  unsigned int size = leaves.size();
  for (unsigned int i = 0; i < size; i++) {
    /*CL_Draw::line(*gc, position() - player_->map_position(),
                  leaves[i]->position() - player_->map_position(),
                  CL_Colorf::green);*/
    leaves[i]->Draw(gc, target);
  }

  // Shoot!
  if (state_ == kShooting && targeting_fly) {
    CL_Draw::line(*gc, position() - target,
                  targeting_fly->position() - target,
                  CL_Colorf::green);
  }


  Plant::Draw(gc, target);
}

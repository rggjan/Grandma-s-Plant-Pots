// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./flower.h"

#include <vector>
#include <list>
#include <algorithm>

#include "bugs/bug.h"
#include "./leaf.h"
#include "plants/plantplayer.h"

#define TIME_TO_OPEN 15000
#define MIN_MASTER_PLANT_DISTANCE 100

#define CO2_COLLECTED_PER_SECOND 0.1
#define SUN_COLLECTED_PER_SECOND 0.01
#define START_ENERGY 30

Flower::Flower(World *world, CL_GraphicContext *gc,
               CL_Vec2f position, PlantPlayer* player)
  : Plant(world, gc, position, "Plant1", player),
    age_(0),
    open_(false) {
  co2_collected_per_second_ = CO2_COLLECTED_PER_SECOND;
  sun_collected_per_second_ = SUN_COLLECTED_PER_SECOND;
  energy_ = START_ENERGY;

  world_->AddMasterPlant(this);
}

Flower::~Flower() {
  world_->RemoveMasterPlant(this);
}

void Flower::AddLeaf(Leaf* leaf) {
  leaves.push_back(leaf);
}

bool Flower::Update(int time_ms) {
  // Update leaves
  remove_if(leaves.begin(), leaves.end(), [time_ms](Leaf * leaf) {
    return !leaf->Update(time_ms);
  });

  if (!is_alive()) {
    return Plant::Update(time_ms) || leaves.size() > 0;
  }

  // Update state
  age_ += time_ms;

  if (!open() && age_ > TIME_TO_OPEN) {
    open_ = true;
    sprite_.set_frame(1);
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
  Plant *nearest_plant = world_->NearestMasterPlant(position);

  if (nearest_plant && (nearest_plant->position() - position).length()
      < MIN_MASTER_PLANT_DISTANCE)
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

  Plant::Draw(gc, target);
}

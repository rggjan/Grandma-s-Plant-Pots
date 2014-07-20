// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./cactus.h"

#include <vector>
#include <algorithm>
#include <list>

#include "bugs/bug.h"
#include "./plantplayer.h"
#include "./leaf.h"

#define TIME_TO_OPEN 15000
#define MIN_MASTER_PLANT_DISTANCE 100

#define CO2_COLLECTED_PER_SECOND 0.01
#define SUN_COLLECTED_PER_SECOND 0.005
#define START_ENERGY 20

clan::SoundBuffer_Session Cactus::sound_session_shot_;

Cactus::Cactus(World *world, clan::Canvas canvas,
               clan::Vec2f position, PlantPlayer* player, bool menu)
  : MasterPlant(world, canvas, position, "Cactus", player, menu),
    age_(0),
    open_(false),
    sound_shot_(clan::SoundBuffer::resource("TowerShoot", world->resources)) {

  sound_shot_.set_volume(0.8f);
  sound_session_shot_ = sound_shot_.prepare();

  co2_collected_per_second_ = CO2_COLLECTED_PER_SECOND;
  sun_collected_per_second_ = SUN_COLLECTED_PER_SECOND;
  energy_ = START_ENERGY;
}

Plant* Cactus::GetNewPlant(clan::Vec2f position, clan::Canvas canvas) {
  return new Cactus(world_, canvas, position, player_);
}

Cactus::~Cactus() {
  world_->RemovePlant(this);
}

void Cactus::AddLeaf(Leaf* leaf) {
  leaves.push_back(leaf);
}

bool Cactus::Update(int time_ms) {
  // Update leaves
  std::remove_if(leaves.begin(), leaves.end(), [time_ms](Leaf * leaf) {
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

Leaf* Cactus::NearestLeaf(clan::Vec2f position) {
  // TODO(rggjan): infinity
  float best_dist = -1;
  Leaf *nearest_leaf = NULL;

  // Get nearest Cactus
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

void Cactus::DrawTmpChild(clan::Canvas *canvas) {
    clan::Vec2f diff = player_->cross_position() -
                    (position() - player_->map_position());

    //float angle = atan2(diff.y, diff.x);
    //tmp_leaf_->set_angle(clan::Angle(angle, clan::radians));

    clan::Colorf line_color;

    /*if (diff.length() < LEAF_MAX_DISTANCE &&
        tmp_leaf_->CanBuild(position(), selected_flower_)) {
      tmp_leaf_->DrawGreen(gc_, cross_position());
      line_color = clan::Colorf::green;
      cross_green_ = true;
    } else {
      tmp_leaf_->DrawRed(gc_, cross_position());*/
      line_color = clan::Colorf::red;
//      cross_green_ = false;
    //}

    diff = position() - player_->map_position();
    canvas->draw_line(diff.x, diff.y, player_->cross_position().x,
    player_->cross_position().y, line_color);
}

void Cactus::Draw(clan::Canvas canvas, clan::Vec2f target) {
  std::list<Leaf *>::iterator it;
  for (it = leaves.begin(); it != leaves.end(); ++it) {
    (*it)->Draw(canvas, target);
    /*clan::Draw::line(*canvas, position() - player_->map_position(),
                  leaves[i]->position() - player_->map_position(),
                  clan::Colorf::green);*/
  }

  if (!is_alive()) {
    Plant::Draw(canvas, target);
    return;
  }

  Plant::Draw(canvas, target);
}

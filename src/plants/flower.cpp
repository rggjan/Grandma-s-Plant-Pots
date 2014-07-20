// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./flower.h"

#include <algorithm>
#include <list>

#include "./plantplayer.h"
#include "./leaf.h"

#define TIME_TO_OPEN 15000

#define CO2_COLLECTED_PER_SECOND 0.1
#define SUN_COLLECTED_PER_SECOND 0.01
#define START_ENERGY 30

#define LEAF_MAX_DISTANCE 80

Flower::Flower(World *world, clan::Canvas canvas,
               clan::Vec2f position, PlantPlayer* player, bool menu)
  : MasterPlant(world, canvas, position, "Flower", player, menu),
    age_(0),
    open_(false),
    menu_leaf_(new Leaf(world, canvas, clan::Vec2f(0, 0), this, true)) {
  co2_collected_per_second_ = CO2_COLLECTED_PER_SECOND;
  sun_collected_per_second_ = SUN_COLLECTED_PER_SECOND;
  energy_ = START_ENERGY;

  // Delete leave where?
}

Plant* Flower::GetNewPlant(clan::Vec2f position, clan::Canvas canvas) {
  return new Flower(world_, canvas, position, player_);
}

Flower::~Flower() {
  world_->RemovePlant(this);
}

void Flower::AddLeaf(Leaf* leaf) {
  leaves.push_back(leaf);
}

bool Flower::Update(int time_ms) {
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

Leaf* Flower::NearestLeaf(clan::Vec2f position) {
  // TODO(rggjan): infinity
  float best_dist = -1;
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

void Flower::DrawTmpChild(clan::Canvas canvas) {
    clan::Vec2f diff = player_->cross_position() -
                    (position() - player_->map_position());

    float angle = atan2(diff.y, diff.x);
    menu_leaf_->set_angle(clan::Angle(angle, clan::angle_radians));

    bool canBuild = diff.length() < LEAF_MAX_DISTANCE;

    clan::Colorf line_color = canBuild ? clan::Colorf::green : clan::Colorf::red;

    menu_leaf_->DrawTmp(canvas, player_->cross_position().x, player_->cross_position().y, 1.0, 1.0, line_color);

    /*if (diff.length() < LEAF_MAX_DISTANCE &&
      menu_leaf_->CanBuild(position(), selected_flower_)) {
      menu_leaf_->DrawGreen(gc_, cross_position());
      line_color = clan::Colorf::green;
      cross_green_ = true;
    } else {
      menu_leaf__->DrawRed(gc_, cross_position());
      line_color = clan::Colorf::red;
//      cross_green_ = false;
    }*/

    diff = position() - player_->map_position();

    // TODO(rggjan): Move to leaf draw
    canvas.draw_line(diff.x, diff.y, player_->cross_position().x,
                     player_->cross_position().y, line_color);
}

void Flower::Draw(clan::Canvas canvas, clan::Vec2f target) {
  std::list<Leaf*>::iterator it;
  for (it = leaves.begin(); it != leaves.end(); ++it) {
    (*it)->Draw(canvas, target);
    canvas.draw_line(position() - target,
                     (*it)->position() - target,
                     clan::Colorf::green);
  }

  if (!is_alive()) {
    Plant::Draw(canvas, target);
    return;
  }

  Plant::Draw(canvas, target);
}

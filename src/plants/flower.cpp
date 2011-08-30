// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./flower.h"

#include <stdlib.h>
#include <ClanLib/display.h>

#include <vector>

#include "./world.h"
#include "./leaf.h"
#include "plants/plantplayer.h"

#define TIME_TO_OPEN 6000
#define TIME_TO_FINAL 15000
#define MIN_FLOWER_DISTANCE 50

Flower::Flower(World *world, CL_GraphicContext *gc,
               CL_Vec2f position, PlantPlayer* player)
  : GameObject(world, position),
    state_(kClosed),
    age_(0),
    player_(player),
    eating_fly_(NULL) {
  spriteImage = new CL_Sprite(*gc, "Plant1", &world->resources);
}

void Flower::AddLeaf(Leaf* leaf) {
  leaves.push_back(leaf);
}

void Flower::DrawRed(CL_GraphicContext *gc, CL_Vec2f position) {
  // Draw graphic
  spriteImage->set_color(CL_Color::red);
  spriteImage->set_alpha(0.8);
  spriteImage->set_frame(2);
  spriteImage->draw(*gc, position.x, position.y);
}

void Flower::DrawGreen(CL_GraphicContext *gc, CL_Vec2f position) {
  // Draw graphic
  spriteImage->set_color(CL_Color::green);
  spriteImage->set_alpha(0.8);
  spriteImage->set_frame(2);
  spriteImage->draw(*gc, position.x, position.y);
}

void Flower::Update(int time_ms) {
  // Update energy from plants
  unsigned int size = leaves.size();
  for (unsigned int i = 0; i < size; i++) {
    player_->increase_energy(leaves[i]->Update(time_ms));
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
      state_ = kFinal;
      spriteImage->set_frame(2);
    }
  }
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

  GameObject::Draw(gc, target);
}

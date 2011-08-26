// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./plantplayer.h"

#include <vector>

#include "./flower.h"
#include "./player.h"
#include "./world.h"
#include "./leaf.h"

using std::vector;

#define LEAF_MAX_DISTANCE 300

PlantPlayer::PlantPlayer(CL_DisplayWindow* window, World* world,
                         int width, int height)
  : Player(window, world, width, height),
    state(Idle) {
  selectedImage = new CL_Sprite(*gc, "Cross2", &world->resources);

  tmpFlower = new Flower(world, gc, CL_Vec2f(0, 0));
  tmpLeaf = new Leaf(world, gc, "Leaf2", CL_Vec2f(0, 0));
}

bool PlantPlayer::BuildPlant() {
  if (energy >= Flower::energy_cost && tmpFlower->CanBuild(position())) {
    Flower *flower = new Flower(world, gc, position());

    flowers.push_back(flower);
    world->addFlower(flower);

    energy -= Flower::energy_cost;
    return true;
  } else {
    // TODO(rggjan): beep
    return false;
  }
}

void PlantPlayer::SelectButtonPressed() {
  switch (state) {
  case Building:
    if (BuildPlant())
      state = Idle;
    break;
  case Idle:
    state = Selecting;
    break;
  case Selecting:
    state = Selected;
    // TODO(rggjan): cache nearestflower
    selectedFlower = NearestFlower();
    break;
  case Selected:
    state = Selecting;
    break;
  default:
    break;
  }
}

void PlantPlayer::CancelButtonPressed() {
  switch (state) {
  case Building:
    state = Idle;
    break;
  case Selecting:
    state = Idle;
    break;
  case Selected:
    state = Selecting;
    break;
  default:
    break;
  }
}

void PlantPlayer::BuildButtonPressed() {
  switch (state) {
  case Idle:
    state = Building;
    break;
  case Building:
    BuildPlant();
    break;
  case Selected:
    state = SelectedBuilding;
    break;
  default:
    break;
  }
}

Flower* PlantPlayer::NearestFlower() {
  // TODO(rggjan): infinity
  int best_dist = -1;
  Flower *nearest_flower = NULL;

  // Get nearest flower
  std::vector<Flower *>::iterator it;
  for (it = flowers.begin(); it != flowers.end(); ++it) {
    float distance = ((*it)->position() - position()).length();

    if (nearest_flower == NULL || distance < best_dist) {
      best_dist = distance;
      nearest_flower = (*it);
    }
  }

  return nearest_flower;
}

void PlantPlayer::Draw() {
  if (state == Selecting) {
    Flower* nearest_flower = NearestFlower();

    selectedImage->set_alpha(0.3);
    if (nearest_flower != NULL) {
      CL_Vec2f pos = nearest_flower->position() - map_position();
      selectedImage->draw(*gc, pos.x, pos.y);
    }
  } else if (state == Selected || state == SelectedBuilding) {
    selectedImage->set_alpha(0.8);

    CL_Vec2f pos = selectedFlower->position() - map_position();
    selectedImage->draw(*gc, pos.x, pos.y);
  }

  Player::Draw();
}

void PlantPlayer::DrawEnergy() {
  switch (state) {
  case Building: {
    CL_Colorf color = CL_Colorf::white;
    if (tmpFlower->energy_cost > energy)
      color = CL_Colorf::red;
    default_font.draw_text(*gc, CL_Pointf(10, 30),
                           cl_format("Energy: %1 (%2)", energy,
                                     tmpFlower->energy_cost), color);
    break;
  }
  default:
    Player::DrawEnergy();
  }
}

void PlantPlayer::draw_cross() {
  switch (state) {
  case Idle:
    Player::draw_cross();
    break;
  case Building:
    if (tmpFlower->CanBuild(position()))
      tmpFlower->DrawGreen(gc, cross_position());
    else
      tmpFlower->DrawRed(gc, cross_position());
    // Player::draw_cross(); TODO(rggjan): better with this?
    break;
  case SelectedBuilding: {
    CL_Vec2f diff = cross_position()-(selectedFlower->position()-map_position());

    float angle = atan2(diff.y, diff.x);
    tmpLeaf->SetAngle(CL_Angle(angle, cl_radians));

    CL_Colorf line_color;

    if (diff.length() > LEAF_MAX_DISTANCE) {
      tmpLeaf->DrawRed(gc, cross_position());
      line_color = CL_Colorf::red;
    } else {
      tmpLeaf->DrawGreen(gc, cross_position());
      line_color = CL_Colorf::green;
    }

    diff = selectedFlower->position() - map_position();
    CL_Draw::line(*gc, diff.x, diff.y, map_position().x, map_position().y,
                  line_color);
    break;
  }
  default: {
    Player::draw_cross();
    break;
  }
  }
}

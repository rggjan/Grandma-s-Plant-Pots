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

  tmpFlower = new Flower(world, gc, 0, 0);
  tmpLeaf = new Leaf(world, gc, "Leaf2", 0, 0);
}

bool PlantPlayer::BuildPlant() {
  if (energy >= Flower::energy_cost && tmpFlower->CanBuild(x(), y())) {
    Flower *flower = new Flower(world, gc, x(), y());

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
    int x_diff = (*it)->posX - x();
    int y_diff = (*it)->posY - y();
    int dist_squared = y_diff*y_diff + x_diff*x_diff;

    if (nearest_flower == NULL || dist_squared < best_dist) {
      best_dist = dist_squared;
      nearest_flower = (*it);
    }
  }

  return nearest_flower;
}

void PlantPlayer::draw() {
  if (state == Selecting) {
    Flower* nearest_flower = NearestFlower();

    selectedImage->set_alpha(0.3);
    if (nearest_flower != NULL)
      selectedImage->draw(*gc, nearest_flower->posX - center_x,
                          nearest_flower->posY - center_y);
  } else if (state == Selected || state == SelectedBuilding) {
    selectedImage->set_alpha(0.8);

    selectedImage->draw(*gc, selectedFlower->posX - center_x,
                        selectedFlower->posY - center_y);
  }

  Player::draw();
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
    if (tmpFlower->CanBuild(x(), y()))
      tmpFlower->DrawGreen(gc, cross_x, cross_y);
    else
      tmpFlower->DrawRed(gc, cross_x, cross_y);
    // Player::draw_cross(); TODO(rggjan): better with this?
    break;
  case SelectedBuilding: {


    float angle = atan2(cross_y - (selectedFlower->posY - center_y),
                        cross_x - (selectedFlower->posX - center_x));

    float diff_x = (selectedFlower->posX - center_x - cross_x);
    float diff_y = (selectedFlower->posY - center_y - cross_y);

    tmpLeaf->SetAngle(CL_Angle(angle, cl_radians));

    CL_Colorf line_color;

    if (diff_x*diff_x + diff_y*diff_y > LEAF_MAX_DISTANCE*LEAF_MAX_DISTANCE) {
      tmpLeaf->DrawRed(gc, cross_x, cross_y);
      line_color = CL_Colorf::red;
    } else {
      tmpLeaf->DrawGreen(gc, cross_x, cross_y);
      line_color = CL_Colorf::green;
    }


    CL_Draw::line(*gc, selectedFlower->posX - center_x,
                  selectedFlower->posY - center_y, cross_x, cross_y,
                  line_color);
    break;
  }
  default: {
    Player::draw_cross();
    break;
  }
  }
}

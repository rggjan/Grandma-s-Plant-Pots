// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./plantplayer.h"

#include <ClanLib/sound.h>
#include <ClanLib/core.h>
#include <vector>

#include <list>

#include "plants/flower.h"
#include "plants/leaf.h"

using std::vector;

#define LEAF_MAX_DISTANCE 80
#define START_CO2 10
#define START_SUGAR 100
#define START_SUN 0

PlantPlayer::PlantPlayer(CL_GraphicContext* gc, World* world,
                         int width, int height)
  : Player(gc, world, width, height),
    co2_(START_CO2),
    sugar_(START_SUGAR),
    sun_(START_SUN),
    state(Idle),
    cross_green_(false) {
  selectedImage = new CL_Sprite(*gc_, "Cross2", &world_->resources);

  tmpFlower = new Flower(world_, gc_, CL_Vec2f(0, 0), this);
  tmpLeaf = new Leaf(world_, gc_, "Leaf2", CL_Vec2f(0, 0), tmpFlower);


  sound_plantgrowing_ =
    new CL_SoundBuffer("PlantgrowingMusic", &world_->resources);
  sound_plantgrowing_->set_volume(1.0f);
  sound_plantgrowing_->prepare();

  sound_leafgrowing_ = new CL_SoundBuffer("LeafgrowingMusic",
                                          &world_->resources);
  sound_leafgrowing_->set_volume(0.1f);
  sound_leafgrowing_->prepare();
}

bool PlantPlayer::BuildLeaf() {
  if (sugar_ >= Leaf::kSugarCost && cross_green_) {
    Leaf *leaf = new Leaf(world_, gc_, "Leaf2", position(), selectedFlower);
    leaf->set_angle(tmpLeaf->angle());

    sugar_ -= Leaf::kSugarCost;
    sound_leafgrowing_->play();
    selectedFlower->AddLeaf(leaf);
    world_->AddPlant(leaf);

    return true;
  } else {
    world_->PlayBeep();
    return false;
  }
}

bool PlantPlayer::BuildFlower() {
  if (sugar_ >= Flower::kSugarCost && cross_green_) {
    Flower *flower = new Flower(world_, gc_, position(), this);

    flowers.push_back(flower);
    world_->AddFlower(flower);

    sugar_ -= Flower::kSugarCost;
    sound_plantgrowing_->play();

    return true;
  } else {
    world_->PlayBeep();
    return false;
  }
}

void PlantPlayer::SelectButtonPressed() {
  switch (state) {
  case Building:
    if (BuildFlower())
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
  case SelectedBuilding:
    if (BuildLeaf())
      state = Idle;
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
  case SelectedBuilding:
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
    BuildFlower();
    break;
  case Selected:
    state = SelectedBuilding;
    break;
  case SelectedBuilding:
    BuildLeaf();
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
  std::list<Flower *>::iterator it;
  for (it = flowers.begin(); it != flowers.end(); ++it) {
    if (!(*it)->is_alive())
      continue;

    float distance = ((*it)->position() - position()).length();

    if (nearest_flower == NULL || distance < best_dist) {
      best_dist = distance;
      nearest_flower = (*it);
    }
  }

  return nearest_flower;
}

void PlantPlayer::DrawFloor() {
  if (state == Selecting) {
    Flower* nearest_flower = NearestFlower();

    selectedImage->set_alpha(0.3);
    if (nearest_flower != NULL) {
      CL_Vec2f pos = nearest_flower->position() - map_position();
      selectedImage->draw(*gc_, pos.x, pos.y);
    }
  } else if (state == Selected || state == SelectedBuilding) {
    selectedImage->set_alpha(0.8);

    CL_Vec2f pos = selectedFlower->position() - map_position();
    selectedImage->draw(*gc_, pos.x, pos.y);
  }
}

void PlantPlayer::DrawEnergy() {
  switch (state) {
  case Building: {
    CL_Colorf color = CL_Colorf::white;
    if (tmpFlower->kSugarCost > sugar_)
      color = CL_Colorf::red;
    default_font_.draw_text(*gc_, CL_Pointf(10, 30),
                           cl_format("Sugar: %1 (%2)",
                                     static_cast<int>(sugar_),
                                     tmpFlower->kSugarCost), color);
    break;
  }
  case SelectedBuilding: {
    CL_Colorf color = CL_Colorf::white;
    if (tmpLeaf->kSugarCost > sugar_)
      color = CL_Colorf::red;
    default_font_.draw_text(*gc_, CL_Pointf(10, 30),
                           cl_format("Sugar: %1 (%2)",
                                     static_cast<int>(sugar_),
                                     tmpLeaf->kSugarCost), color);
    break;
  }
  default: {
    default_font_.draw_text(*gc_, CL_Pointf(10, 30),
                           cl_format("Sugar: %1", static_cast<int>(sugar_)),
                           CL_Colorf::white);
  }
  }
}

void PlantPlayer::DrawCO2() {
  default_font_.draw_text(*gc_, CL_Pointf(10, 50),
                         cl_format("CO2: %1",  static_cast<int>(co2_)),
                         CL_Colorf::white);
}

void PlantPlayer::DrawSun() {
  default_font_.draw_text(*gc_, CL_Pointf(10, 70),
                         cl_format("Sun: %1",  static_cast<int>(sun_ * 60)),
                         CL_Colorf::white);
}

void PlantPlayer::Update(int time_ms) {
  // Set sun to zero... will be added up in the update function!
  sun_ = 0;
  std::list<Flower *>::iterator it;
  for (it = flowers.begin(); it != flowers.end();) {
    if (!(*it)->Update(time_ms)) {
      world_->RemoveFlower(*it);
      delete *it;
      it = flowers.erase(it);
    } else {
      ++it;
    }
  }

  // Produce sugar
  double sugar_production = sun_ * time_ms / 1000.;
  if (co2_ < sun_)
    sugar_production = co2_;

  co2_ -= sugar_production;
  sugar_ += sugar_production;

  Player::Update(time_ms);
}

void PlantPlayer::DrawTop() {
  switch (state) {
  case Idle:
    Player::DrawTop();
    break;
  case Building:
    if (tmpFlower->CanBuild(position())) {
      tmpFlower->DrawGreen(gc_, cross_position());
      cross_green_ = true;
    } else {
      tmpFlower->DrawRed(gc_, cross_position());
      cross_green_ = false;
    }
    // Player::draw_cross(); TODO(rggjan): better with this?
    break;
  case SelectedBuilding: {
    CL_Vec2f diff = cross_position() -
                    (selectedFlower->position() - map_position());

    float angle = atan2(diff.y, diff.x);
    tmpLeaf->set_angle(CL_Angle(angle, cl_radians));

    CL_Colorf line_color;

    if (diff.length() < LEAF_MAX_DISTANCE &&
        tmpLeaf->CanBuild(position(), selectedFlower)) {
      tmpLeaf->DrawGreen(gc_, cross_position());
      line_color = CL_Colorf::green;
      cross_green_ = true;
    } else {
      tmpLeaf->DrawRed(gc_, cross_position());
      line_color = CL_Colorf::red;
      cross_green_ = false;
    }

    diff = selectedFlower->position() - map_position();
    CL_Draw::line(*gc_, diff.x, diff.y, cross_position().x, cross_position().y,
                  line_color);
    break;
  }
  default: {
    Player::DrawTop();
    break;
  }
  }

  DrawEnergy();
  DrawCO2();
  DrawSun();
}

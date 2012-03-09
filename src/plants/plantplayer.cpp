// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./plantplayer.h"

#include <ClanLib/core.h>
#include <vector>

#include <list>
#include <algorithm>

#include "plants/flower.h"
#include "plants/leaf.h"
#include "plants/tower.h"

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
    select_sprite_(*gc_, "Cross2", &world_->resources),
    menu_item_(0),    
    sound_plantgrowing_("PlantgrowingMusic", &world_->resources),
    sound_leafgrowing_("LeafgrowingMusic", &world_->resources) {
  //tmp_plant_ = new Tower(world_, gc_, CL_Vec2f(0, 0), this);
  plant_menu_.push_back(new Tower(world_, gc_, CL_Vec2f(0, 0), this));
  plant_menu_.push_back(new Flower(world_, gc_, CL_Vec2f(0, 0), this));
  world->RemoveMasterPlant(plant_menu_[0]);
  world->RemoveMasterPlant(plant_menu_[1]);

  //tmp_leaf_ = new Leaf(world_, gc_, CL_Vec2f(0, 0), "Leaf2", tmp_flower_);
  //world->RemovePlant(tmp_leaf_);

  sound_plantgrowing_.set_volume(1.0f);
  sound_plantgrowing_.prepare();

  sound_leafgrowing_.set_volume(0.1f);
  sound_leafgrowing_.prepare();
}

PlantPlayer::~PlantPlayer() {
  //delete tmp_plant_;
  //delete tmp_leaf_;
}

bool PlantPlayer::BuildLeaf() {
  /*if (sugar_ >= Leaf::kSugarCost && cross_green_) {
    Leaf *leaf = new Leaf(world_, gc_, position(), "Leaf2", selected_flower_);
    leaf->set_angle(tmp_leaf_->angle());

    sugar_ -= Leaf::kSugarCost;
    sound_leafgrowing_.play();
    selected_flower_->AddLeaf(leaf);

    return true;
  } else {
    world_->PlayBeep();
    return false;
  }*/
  return false;
}

bool PlantPlayer::BuildPlant(Plant *plant) {
  if (plant->CanBuild(position())) {
    //plants_.push_back()
    plants_.push_back(plant->GetNewPlant(position(), gc_));

    sugar_ -= plant->sugar_cost();
    sound_plantgrowing_.play();

    return true;
  } else {
    world_->PlayBeep();
    return false;
  }
}

void PlantPlayer::SelectButtonPressed() {
  switch (state) {
  case Building:
    if (BuildPlant(plant_menu_[menu_item_]))
      state = Idle;
    break;
  /*case Idle:
    state = Selecting;
    break;
  case Selecting:
    selected_flower_ = NearestFlower();
    // TODO(rggjan): cache nearestflower
    if (selected_flower_ != NULL)
      state = Selected;
    break;
  case Selected:
    state = Selecting;
    break;
  case SelectedBuilding:
    if (BuildLeaf())
      state = Idle;*/
  default:
    break;
  }
}

void PlantPlayer::CancelButtonPressed() {
  switch (state) {
  case Idle:
    break;
  case BuildMenu:
    state = Idle;
    break;
  case Building:
    state = BuildMenu;
    break;
  /*case Selecting:
    state = Idle;
    break;
  case Selected:
    state = Selecting;
    break;
  case SelectedBuilding:
    state = Selecting;
    break;
  default:
    break;*/
  }
}

void PlantPlayer::BuildButtonPressed() {
  switch (state) {
  case Idle:
    state = BuildMenu;
    break;
  case BuildMenu:
    BuildPlant(plant_menu_[menu_item_]);
    state = Building;
    break;
  case Building:
    BuildPlant(plant_menu_[menu_item_]);
    break;
  /*case Selected:
    state = SelectedBuilding;
    break;
  case SelectedBuilding:
    BuildLeaf();
    break;*/
  default:
    break;
  }
}

Flower* PlantPlayer::NearestFlower() {
  // TODO(rggjan): infinity
  int best_dist = -1;
  Flower *nearest_flower = NULL;
  /*
    // Get nearest flower
  for (Tower * flower : flowers) {
      if (!flower->is_alive())
        continue;

      float distance = (flower->position() - position()).length();

      if (nearest_flower == NULL || distance < best_dist) {
        best_dist = distance;
        nearest_flower = flower;
      }
    }*/

  return nearest_flower;
}

void PlantPlayer::DrawFloor() {
  /*if (state == Selecting) {
    Flower* nearest_flower = NearestFlower();

    select_sprite_.set_alpha(0.3);
    if (nearest_flower != NULL) {
      CL_Vec2f pos = nearest_flower->position() - map_position();
      select_sprite_.draw(*gc_, pos.x, pos.y);
    }
  } else if (state == Selected || state == SelectedBuilding) {
    select_sprite_.set_alpha(0.8);

    CL_Vec2f pos = selected_flower_->position() - map_position();
    select_sprite_.draw(*gc_, pos.x, pos.y);
  }*/
}

void PlantPlayer::DrawEnergy() {
  /*switch (state) {
  case Building: {
    CL_Colorf color = CL_Colorf::white;
    if (tmp_flower_->kSugarCost > sugar_)
      color = CL_Colorf::red;
    default_font_.draw_text(*gc_, CL_Pointf(10, 30),
                            cl_format("Sugar: %1 (%2)",
                                      static_cast<int>(sugar_),
                                      tmp_flower_->kSugarCost), color);
    break;
  }
  case SelectedBuilding: {
    CL_Colorf color = CL_Colorf::white;
    if (tmp_leaf_->kSugarCost > sugar_)
      color = CL_Colorf::red;
    default_font_.draw_text(*gc_, CL_Pointf(10, 30),
                            cl_format("Sugar: %1 (%2)",
                                      static_cast<int>(sugar_),
                                      tmp_leaf_->kSugarCost), color);
    break;
  }
  default: {
    default_font_.draw_text(*gc_, CL_Pointf(10, 30),
                            cl_format("Sugar: %1", static_cast<int>(sugar_)),
                            CL_Colorf::white);
  }
  }*/
}

void PlantPlayer::MovingLeftButtonPressed() {
  if (state == BuildMenu)
    menu_item_ = (menu_item_+(plant_menu_.size()-1))%plant_menu_.size();
  else
    Player::MovingLeftButtonPressed();
}

void PlantPlayer::MovingRightButtonPressed() {
  if (state == BuildMenu)
    menu_item_ = (menu_item_+1)%plant_menu_.size();
  else
    Player::MovingRightButtonPressed();
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

  // Remove dead and invisible plants
  remove_if(plants_.begin(), plants_.end(), [time_ms](Plant * plant) {
    return !plant->Update(time_ms);
  });

  // Produce sugar
  double sugar_production = sun_ * time_ms / 1000.;
  if (co2_ < sun_)
    sugar_production = co2_;
  co2_ -= sugar_production;
  sugar_ += sugar_production;

  if (state != BuildMenu)
    Player::Update(time_ms);
}

bool PlantPlayer::CanBuild(Plant *plant) {
  return (sugar_ >= plant->sugar_cost() && plant->CanBuild(position()));
}

void PlantPlayer::DrawTop() {
  switch (state) {
  case Idle:
    Player::DrawTop();
    break;
    case BuildMenu:
    case Building:
      plant_menu_[menu_item_]->DrawTmp(gc_, CanBuild(plant_menu_[menu_item_]));
      // Player::draw_cross(); TODO(rggjan): better with this?
      break;
    /*case SelectedBuilding: {
      CL_Vec2f diff = cross_position() -
                      (selected_flower_->position() - map_position());

      float angle = atan2(diff.y, diff.x);
      tmp_leaf_->set_angle(CL_Angle(angle, cl_radians));

      CL_Colorf line_color;

      if (diff.length() < LEAF_MAX_DISTANCE &&
          tmp_leaf_->CanBuild(position(), selected_flower_)) {
        tmp_leaf_->DrawGreen(gc_, cross_position());
        line_color = CL_Colorf::green;
        cross_green_ = true;
      } else {
        tmp_leaf_->DrawRed(gc_, cross_position());
        line_color = CL_Colorf::red;
        cross_green_ = false;
      }

      diff = selected_flower_->position() - map_position();
      CL_Draw::line(*gc_, diff.x, diff.y, cross_position().x, cross_position().y,
                    line_color);
      break;
    }*/
  default: {
    Player::DrawTop();
    break;
  }
  }

  DrawEnergy();
  DrawCO2();
  DrawSun();
}

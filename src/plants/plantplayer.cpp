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

PlantPlayer::PlantPlayer(clan::Canvas* canvas, World* world,
                         clan::Sprite map)
  : Player(canvas, world, map),
    co2_(START_CO2),
    sugar_(START_SUGAR),
    sun_(START_SUN),
    state(Idle),
    select_sprite_(clan::Sprite::resource(*gc_, "Cross2", world_->resources)),
    menu_item_(0),    
    sound_plantgrowing_(clan::SoundBuffer::resource("PlantgrowingMusic", world_->resources)),
    sound_leafgrowing_(clan::SoundBuffer::resource("LeafgrowingMusic", world_->resources)) {
  //tmp_plant_ = new Tower(world_, gc_, clan::Vec2f(0, 0), this);
  plant_menu_.push_back(new Tower(world_, gc_, clan::Vec2f(0, 0), this, true));
  plant_menu_.push_back(new Flower(world_, gc_, clan::Vec2f(0, 0), this, true));

  //tmp_leaf_ = new Leaf(world_, gc_, clan::Vec2f(0, 0), "Leaf2", tmp_flower_);
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
  case BuildMenu:
  case Building:
    if (BuildPlant(plant_menu_[menu_item_]))
      state = Idle;
    break;
  case Idle:
    state = Selecting;
    break;
  case Selecting:
    selected_plant_ = NearestPlant();
    // TODO(rggjan): cache nearestflower
    if (selected_plant_ != NULL)
      state = Selected;
    break;
  /*case Selected:
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
  case Building:
  case Selecting:
  case SelectedBuilding:
    state = Idle;
    break;
  case Selected:
    state = Selecting;
    break;
  /*case SelectedBuilding:
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
  case Selected:
    if (selected_plant_->has_children())
      state = SelectedBuilding;
    else
      world_->PlayBeep();        
    break;
  /*case SelectedBuilding:
    BuildLeaf();
    break;*/
  default:
    break;
  }
}

Plant* PlantPlayer::NearestPlant() {
  // TODO(rggjan): infinity
  int best_dist = -1;
  Plant *nearest_plant = NULL;

  // Get nearest flower
  std::list<Plant*>::iterator plant;
  for (plant = plants_.begin(); plant != plants_.end(); ++plant) {
      if (!(*plant)->is_alive())
        continue;

      float distance = ((*plant)->position() - position()).length();

      if (nearest_plant == NULL || distance < best_dist) {
        best_dist = distance;
        nearest_plant = *plant;
      }
    }

  return nearest_plant;
}

void PlantPlayer::DrawFloor() {
  Player::DrawFloor();

  if (state == Selecting) {
    Plant* nearest_plant = NearestPlant();

    select_sprite_.set_alpha(0.3);
    if (nearest_plant != NULL) {
      printf("feund nearest flower\n");
      clan::Vec2f pos = nearest_plant->position() - map_position();
      select_sprite_.draw(*gc_, pos.x, pos.y);
    }
  } else if (state == Selected /*|| state == SelectedBuilding*/) {
    select_sprite_.set_alpha(0.8);

    clan::Vec2f pos = selected_plant_->position() - map_position();
    select_sprite_.draw(*gc_, pos.x, pos.y);
  }
}

void PlantPlayer::DrawEnergy() {
  /*switch (state) {
  case Building: {
    clan::Colorf color = clan::Colorf::white;
    if (tmp_flower_->kSugarCost > sugar_)
      color = clan::Colorf::red;
    default_font_.draw_text(*gc_, clan::Pointf(10, 30),
                            clan::string_format("Sugar: %1 (%2)",
                                      static_cast<int>(sugar_),
                                      tmp_flower_->kSugarCost), color);
    break;
  }
  case SelectedBuilding: {
    clan::Colorf color = clan::Colorf::white;
    if (tmp_leaf_->kSugarCost > sugar_)
      color = clan::Colorf::red;
    default_font_.draw_text(*gc_, clan::Pointf(10, 30),
                            clan::string_format("Sugar: %1 (%2)",
                                      static_cast<int>(sugar_),
                                      tmp_leaf_->kSugarCost), color);
    break;
  }
  default: {
    default_font_.draw_text(*gc_, clan::Pointf(10, 30),
                            clan::string_format("Sugar: %1", static_cast<int>(sugar_)),
                            clan::Colorf::white);
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
  default_font_.draw_text(*gc_, clan::Pointf(10, 50),
                          clan::string_format("CO2: %1",  static_cast<int>(co2_)),
                          clan::Colorf::white);
}

void PlantPlayer::DrawSun() {
  default_font_.draw_text(*gc_, clan::Pointf(10, 70),
                          clan::string_format("Sun: %1",  static_cast<int>(sun_ * 60)),
                          clan::Colorf::white);
}

void PlantPlayer::Update(int time_ms) {
  // Set sun to zero... will be added up in the update function!
  sun_ = 0;

  // Remove dead and invisible plants
  std::remove_if(plants_.begin(), plants_.end(), [time_ms](Plant * plant) {
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
  return sugar_ >= plant->sugar_cost();
}

void PlantPlayer::DrawTop() {
  switch (state) {
  case Idle:
    Player::DrawTop();
    break;
    case BuildMenu:
    case Building:
      plant_menu_[menu_item_]->DrawTmp(gc_);
      // Player::draw_cross(); TODO(rggjan): better with this?
      break;
    case SelectedBuilding:
      selected_plant_->DrawTmpChild(gc_);

/* {
      clan::Vec2f diff = cross_position() -
                      (selected_flower_->position() - map_position());

      float angle = atan2(diff.y, diff.x);
      tmp_leaf_->set_angle(clan::Angle(angle, clan::radians));

      clan::Colorf line_color;

      if (diff.length() < LEAF_MAX_DISTANCE &&
          tmp_leaf_->CanBuild(position(), selected_flower_)) {
        tmp_leaf_->DrawGreen(gc_, cross_position());
        line_color = clan::Colorf::green;
        cross_green_ = true;
      } else {
        tmp_leaf_->DrawRed(gc_, cross_position());
        line_color = clan::Colorf::red;
        cross_green_ = false;
      }

      diff = selected_flower_->position() - map_position();
      clan::Draw::line(*gc_, diff.x, diff.y, cross_position().x, cross_position().y,
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

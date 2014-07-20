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
#define START_IRON 1000

PlantPlayer::PlantPlayer(clan::Canvas canvas, World* world,
                         clan::Sprite map, clan::Size window_size)
  : Player(canvas, world, map, window_size),
    co2_(START_CO2),
    sugar_(START_SUGAR),
    sun_(START_SUN),
    iron_(START_IRON),
    state_(Idle),
    select_sprite_(clan::Sprite::resource(canvas_, "Cross2", world_->resources)),
    co2_sprite_(clan::Sprite::resource(canvas_, "Co2", world_->resources)),
    sun_sprite_(clan::Sprite::resource(canvas_, "Sun", world_->resources)),
    sugar_sprite_(clan::Sprite::resource(canvas_, "Sugar", world_->resources)),
    iron_sprite_(clan::Sprite::resource(canvas_, "Iron", world_->resources)),
    menu_item_(0),    
    sound_plantgrowing_(clan::SoundBuffer::resource("PlantgrowingMusic", world_->resources)),
    sound_leafgrowing_(clan::SoundBuffer::resource("LeafgrowingMusic", world_->resources)) {
  //tmp_plant_ = new Tower(world_, gc_, clan::Vec2f(0, 0), this);
  plant_menu_.push_back(new Tower(world_, canvas_, clan::Vec2f(0, 0), this, true));
  plant_menu_.push_back(new Flower(world_, canvas_, clan::Vec2f(0, 0), this, true));
  plant_menu_.push_back(new Flower(world_, canvas_, clan::Vec2f(0, 0), this, true));

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
    plants_.push_back(plant->GetNewPlant(position(), canvas_));

    sugar_ -= plant->sugar_cost();
    sound_plantgrowing_.play();

    return true;
  } else {
    world_->PlayBeep();
    return false;
  }
}

void PlantPlayer::SelectButtonPressed() {
  switch (state_) {
  case BuildMenu:
  case Building:
    if (BuildPlant(plant_menu_[menu_item_]))
      state_ = Idle;
    break;
  case Idle:
    state_ = Selecting;
    break;
  case Selecting:
    selected_plant_ = NearestPlant();
    // TODO(rggjan): cache nearestflower
    if (selected_plant_ != NULL)
      state_ = Selected;
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
  switch (state_) {
  case Idle:
    break;
  case BuildMenu:
  case Building:
  case Selecting:
  case SelectedBuilding:
    state_ = Idle;
    break;
  case Selected:
    state_ = Selecting;
    break;
  /*case SelectedBuilding:
    state = Selecting;
    break;
  default:
    break;*/
  }
}

void PlantPlayer::BuildButtonPressed() {
  switch (state_) {
  case Idle:
    state_ = BuildMenu;
    break;
  case BuildMenu:
    BuildPlant(plant_menu_[menu_item_]);
    state_ = Building;
    break;
  case Building:
    BuildPlant(plant_menu_[menu_item_]);
    break;
  case Selected:
    if (selected_plant_->has_children())
      state_ = SelectedBuilding;
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

  if (state_ == Selecting) {
    Plant* nearest_plant = NearestPlant();

    select_sprite_.set_alpha(0.3);
    if (nearest_plant != NULL) {
      clan::Vec2f pos = nearest_plant->position() - map_position();
      select_sprite_.draw(canvas_, pos.x, pos.y);
    }
  } else if (state_ == Selected /*|| state == SelectedBuilding*/) {
    select_sprite_.set_alpha(0.8);

    clan::Vec2f pos = selected_plant_->position() - map_position();
    select_sprite_.draw(canvas_, pos.x, pos.y);
  }
}

void PlantPlayer::DrawEnergy() {
  sugar_sprite_.draw(canvas_, 310,5);
  default_font_.draw_text(canvas_, clan::Pointf(350, 30),
                          clan::string_format("Sugar: %1",  static_cast<int>(sugar_)),
                          clan::Colorf::white);


  if(state_ == BuildMenu || state_ == Building)
  {
    if (plant_menu_[menu_item_]->sugar_cost() > sugar_)
      small_font_.draw_text(canvas_, clan::Pointf(350, 48),
                            clan::string_format("Cost: %1",  plant_menu_[menu_item_]->sugar_cost()),
                            clan::Colorf::red);

    else
      small_font_.draw_text(canvas_, clan::Pointf(350, 48),
                            clan::string_format("Cost: %1",  plant_menu_[menu_item_]->sugar_cost()),
                            clan::Colorf::green);
  }

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
  if (state_ == BuildMenu)
    menu_item_ = (menu_item_+(plant_menu_.size()-1))%plant_menu_.size();
  else
    Player::MovingLeftButtonPressed();
}

void PlantPlayer::MovingRightButtonPressed() {
  if (state_ == BuildMenu)
    menu_item_ = (menu_item_+1)%plant_menu_.size();
  else
    Player::MovingRightButtonPressed();
}

void PlantPlayer::DrawCO2() {
  co2_sprite_.draw(canvas_, 10,5);
  default_font_.draw_text(canvas_, clan::Pointf(50, 30),
                          clan::string_format("CO2: %1",  static_cast<int>(co2_)),
                          clan::Colorf::white);


}

void PlantPlayer::DrawSun() {
    sun_sprite_.draw(canvas_, 160,5);
  default_font_.draw_text(canvas_, clan::Pointf(200, 30),
                          clan::string_format("Sun: %1",  static_cast<int>(sun_ * 60)),
                          clan::Colorf::white);
}

void PlantPlayer::DrawIron() {
  iron_sprite_.draw(canvas_, 460,5);
  default_font_.draw_text(canvas_, clan::Pointf(500, 30),
                          clan::string_format("Iron: %1",  static_cast<int>(iron_)),
                          clan::Colorf::white);
  if(state_ == BuildMenu || state_ == Building)
  {
    if (plant_menu_[menu_item_]->iron_cost() > iron_)
      small_font_.draw_text(canvas_, clan::Pointf(500, 48),
                            clan::string_format("Cost: %1",  plant_menu_[menu_item_]->iron_cost()),
                            clan::Colorf::red);
    else
      small_font_.draw_text(canvas_, clan::Pointf(500, 48),
                            clan::string_format("Cost: %1",  plant_menu_[menu_item_]->iron_cost()),
                            clan::Colorf::green);
  }
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

  if (state_ != BuildMenu)
    Player::Update(time_ms);
}

bool PlantPlayer::CanBuild(Plant *plant) {
  return sugar_ >= plant->sugar_cost();
}

void PlantPlayer::DrawTop() {
  switch (state_) {
  case Idle:
    Player::DrawTop();
    break;
    case BuildMenu:
      canvas_.fill_rect(0,window_size_.height, window_size_.width, window_size_.height-70, clan::Colorf::silver);
      canvas_.fill_rect(50-30+(menu_item_*70),window_size_.height-35-30, 50+30+(menu_item_*70),window_size_.height-5, clan::Colorf::grey);
      for (size_t i=0; i<plant_menu_.size(); i++) {
        plant_menu_[i]->DrawTmp(canvas_,50+(i*70), window_size_.height-35, 1.0f, i== menu_item_ ? 0.6f : 0.5f);
      }



    case Building:
      plant_menu_[menu_item_]->DrawTmp(canvas_, cross_position().x, cross_position().y, 0.8f, 1.0f, plant_menu_[menu_item_]->CanBuild(position()) ? clan::Color::green : clan::Color::red);
      break;
    case SelectedBuilding:
      selected_plant_->DrawTmpChild(canvas_);

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
  DrawIron();
}

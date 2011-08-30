// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./plantplayer.h"

#include <ClanLib/sound.h>
#include <vector>

#include "./flower.h"
#include "./player.h"
#include "./world.h"
#include "./leaf.h"

using std::vector;

#define LEAF_MAX_DISTANCE 80
#define START_ENERGY 100

PlantPlayer::PlantPlayer(CL_DisplayWindow* window, World* world,
                         int width, int height)
  : Player(window, world, width, height),
    state(Idle),
    cross_green_(false),
    energy_(START_ENERGY) {
  selectedImage = new CL_Sprite(*gc, "Cross2", &world->resources);

  tmpFlower = new Flower(world, gc, CL_Vec2f(0, 0), this);
  tmpLeaf = new Leaf(world, gc, "Leaf2", CL_Vec2f(0, 0), tmpFlower);


  sound_plantgrowing_ =
    new CL_SoundBuffer("PlantgrowingMusic", &world->resources);
  sound_plantgrowing_->set_volume(1.0f);
  sound_plantgrowing_->prepare();

  sound_beep1_ = new CL_SoundBuffer("Beep1Music", &world->resources);
  sound_beep1_->set_volume(0.1f);
  sound_beep1_->prepare();
}

bool PlantPlayer::BuildLeaf() {
  if (energy() >= Leaf::energy_cost && cross_green_) {
    Leaf *leaf = new Leaf(world, gc, "Leaf2", position(), selectedFlower);
    leaf->set_angle(tmpLeaf->angle());

    energy_ -= Leaf::energy_cost;
    selectedFlower->AddLeaf(leaf);

    return true;
  } else {
    sound_beep1_->play();
    return false;
  }
}

bool PlantPlayer::BuildPlant() {
  if (energy() >= Flower::energy_cost && cross_green_) {
    Flower *flower = new Flower(world, gc, position(), this);

    flowers.push_back(flower);
    world->addFlower(flower);

    energy_ -= Flower::energy_cost;
    sound_plantgrowing_->play();

    return true;
  } else {
    sound_beep1_->play();
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
    BuildPlant();
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

void PlantPlayer::DrawFloor() {
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
}

void PlantPlayer::DrawEnergy() {
  switch (state) {
  case Building: {
    CL_Colorf color = CL_Colorf::white;
    if (tmpFlower->energy_cost > energy_)
      color = CL_Colorf::red;
    default_font.draw_text(*gc, CL_Pointf(10, 30),
                           cl_format("Energy: %1 (%2)",
                                     static_cast<int>(energy_),
                                     tmpFlower->energy_cost), color);
    break;
  }
  case SelectedBuilding: {
    CL_Colorf color = CL_Colorf::white;
    if (tmpLeaf->energy_cost > energy_)
      color = CL_Colorf::red;
    default_font.draw_text(*gc, CL_Pointf(10, 30),
                           cl_format("Energy: %1 (%2)",
                                     static_cast<int>(energy_),
                                     tmpLeaf->energy_cost), color);
    break;
  }
  default: {
    default_font.draw_text(*gc, CL_Pointf(10, 30),
                           cl_format("Energy: %1", static_cast<int>(energy_)),
                           CL_Colorf::white);
  }
  }
}

void PlantPlayer::Update(int time_ms) {
  for (unsigned int i = 0; i < flowers.size(); i++) {
    flowers[i]->Update(time_ms);
  }

  Player::Update(time_ms);
}

void PlantPlayer::DrawTop() {
  switch (state) {
  case Idle:
    Player::DrawTop();
    break;
  case Building:
    if (tmpFlower->CanBuild(position())) {
      tmpFlower->DrawGreen(gc, cross_position());
      cross_green_ = true;
    } else {
      tmpFlower->DrawRed(gc, cross_position());
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
      tmpLeaf->DrawGreen(gc, cross_position());
      line_color = CL_Colorf::green;
      cross_green_ = true;
    } else {
      tmpLeaf->DrawRed(gc, cross_position());
      line_color = CL_Colorf::red;
      cross_green_ = false;
    }

    diff = selectedFlower->position() - map_position();
    CL_Draw::line(*gc, diff.x, diff.y, cross_position().x, cross_position().y,
                  line_color);
    break;
  }
  default: {
    Player::DrawTop();
    break;
  }
  }

  DrawEnergy();
}

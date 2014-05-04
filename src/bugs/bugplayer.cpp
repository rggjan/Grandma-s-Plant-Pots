// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "bugs/bugplayer.h"

#include <ClanLib/core.h>
#include <list>
#include <algorithm>

#include "./world.h"
#include "plants/plant.h"

#define NUM_BUGS 6
#define START_ENERGY 10

using std::list;
using std::count_if;

BugPlayer::BugPlayer(clan::Canvas canvas, World* world,
                     clan::Sprite map, clan::Size window_size)
  : Player(canvas, world, map, window_size),
    select_sprite_(clan::Sprite::resource(canvas_, "Cross2", world->resources)),
    count_sprite_(clan::Sprite::resource(canvas_, "Count", world_->resources)),
    sound_bug_attack_(clan::SoundBuffer::resource("BugAttack", world->resources)) {
  select_sprite_.set_alpha(0.5);

  for (int i = 0; i < NUM_BUGS; i++) {
    const char* name;

    if (i % 2 == 0)
      name = "Bug1";
    else
      name = "Bug2";

    CreateBug(name, clan::Vec2f(i * 60, i * 60));
  }

  sound_bug_attack_.set_volume(0.1f);
  sound_bug_attack_.prepare();
}

void BugPlayer::CreateBug(std::string name, clan::Vec2f position) {
  AddBug(new Bug(world_, canvas_, position, name, this));
}

void BugPlayer::AddBug(Bug* bug) {
  bugs.push_back(bug);
}

void BugPlayer::SelectButtonPressed() {
  Bug* bug = GetFreeBug();

  if (bug != NULL && nearest_free_plant_ != NULL) {
    sound_bug_attack_.play();
    bug->SetTargetPlant(nearest_free_plant_);
  } else {
    world_->PlayBeep();
  }
}

void BugPlayer::CancelButtonPressed() {
   std::for_each(bugs.begin(), bugs.end(), [](Bug * bug) {
    bug->StopEating();
  });
}

void BugPlayer::BuildButtonPressed() {
}

Plant* BugPlayer::GetFreePlant() {
  list<Plant*>* plants = world_->NearestPlants(position());
  list<Plant*>::iterator plant;
  for (plant = plants->begin(); plant != plants->end(); ++plant) {
    if ((*plant)->is_alive() && (*plant)->free_space() && Visible((*plant)->position()))
      return *plant;
  }

  return NULL;
}

Bug* BugPlayer::GetFreeBug() {
  list<Bug*>::iterator bug;
  for (bug = bugs.begin(); bug != bugs.end(); ++bug)
    if ((*bug)->is_alive() && (*bug)->is_free())
      return *bug;

  return NULL;
}

void BugPlayer::DrawFloor() {
  Player::DrawFloor();

  if (nearest_free_plant_ != NULL) {
    clan::Vec2f pos = nearest_free_plant_->position() - map_position();
    select_sprite_.draw(canvas_, pos.x, pos.y);
  }
  /*  } else if (state == Selected || state == SelectedBuilding) {
      select_sprite_.set_alpha(0.8);

      clan::Vec2f pos = selectedFlower->position() - map_position();
      select_sprite_.draw(*canvas, pos.x, pos.y);
    }*/
}

bool IsAlive(Bug *bug) {
  return bug->is_alive();
}

void BugPlayer::DrawTop() {
  int size = count_if(bugs.begin(), bugs.end(), IsAlive);

  clan::Colorf color = clan::Colorf::white;
  count_sprite_.draw(canvas_, 10,5);
  default_font_.draw_text(canvas_, clan::Pointf(50, 30),
                          clan::string_format("Bugs: %1", size), color);
  Player::DrawTop();
}

void BugPlayer::Update(int time_ms) {
  std::list<Bug *>::iterator it;
  for (it = bugs.begin(); it != bugs.end();) {
    Bug* bug = *it;

    if (!bug->Update(time_ms, position())) {
      delete bug;
      it = bugs.erase(it);
    } else {
      ++it;
    }
  }

  nearest_free_plant_ = GetFreePlant();

  Player::Update(time_ms);
}

// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./cactus.h"

#include <vector>
#include <algorithm>
#include <list>

#include "bugs/bug.h"
#include "./plantplayer.h"
#include "./leaf.h"

#define TIME_TO_OPEN 15000
#define MIN_MASTER_PLANT_DISTANCE 100

#define CO2_COLLECTED_PER_SECOND 0.02
#define SUN_COLLECTED_PER_SECOND 0.1
#define START_ENERGY 40

clan::SoundBuffer_Session Cactus::sound_session_shot_;

Cactus::Cactus(World *world, clan::Canvas canvas,
               clan::Vec2f position, PlantPlayer* player, bool menu)
  : MasterPlant(world, canvas, position, "Cactus", player, menu),
    age_(0),
    open_(false),
    sound_shot_(clan::SoundBuffer::resource("TowerShoot", world->resources)) {

  sound_shot_.set_volume(0.8f);
  sound_session_shot_ = sound_shot_.prepare();

  co2_collected_per_second_ = CO2_COLLECTED_PER_SECOND;
  sun_collected_per_second_ = SUN_COLLECTED_PER_SECOND;
  energy_ = start_energy_ = START_ENERGY;
}

Plant* Cactus::GetNewPlant(clan::Vec2f position, clan::Canvas canvas) {
  return new Cactus(world_, canvas, position, player_);
}

Cactus::~Cactus() {
  world_->RemovePlant(this);
}



bool Cactus::Update(int time_ms) {


  if (!is_alive()) {
    return Plant::Update(time_ms);
  }

  // Update state
  age_ += time_ms;

  if (!open() && age_ > TIME_TO_OPEN) {
    open_ = true;
    sprite_.set_frame(1);
  }

  return Plant::Update(time_ms);
}


void Cactus::DrawTmpChild(clan::Canvas *canvas) {
    clan::Vec2f diff = player_->cross_position() -
                    (position() - player_->map_position());

    //float angle = atan2(diff.y, diff.x);
    //tmp_leaf_->set_angle(clan::Angle(angle, clan::radians));

    clan::Colorf line_color;

    /*if (diff.length() < LEAF_MAX_DISTANCE &&
        tmp_leaf_->CanBuild(position(), selected_flower_)) {
      tmp_leaf_->DrawGreen(gc_, cross_position());
      line_color = clan::Colorf::green;
      cross_green_ = true;
    } else {
      tmp_leaf_->DrawRed(gc_, cross_position());*/
      line_color = clan::Colorf::red;
//      cross_green_ = false;
    //}

    diff = position() - player_->map_position();
    canvas->draw_line(diff.x, diff.y, player_->cross_position().x,
    player_->cross_position().y, line_color);
}

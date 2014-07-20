// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./masterplant.h"
#include "./plantplayer.h"

#define MIN_MASTER_PLANT_DISTANCE 100

MasterPlant::MasterPlant(World *world, clan::Canvas canvas, clan::Vec2f position, std::string name, PlantPlayer *player, bool menu)
  : Plant(world, canvas, position, name, player, menu)
{
}

bool MasterPlant::CanBuild(clan::Vec2f position) {
  Plant *nearest_plant = world_->NearestMasterPlant(position);

  if (nearest_plant && (nearest_plant->position() - position).length()
      < MIN_MASTER_PLANT_DISTANCE)
    return false;

  if (!player_->CanBuild(this))
    return false;

  return world_->CanBuild(position);
}

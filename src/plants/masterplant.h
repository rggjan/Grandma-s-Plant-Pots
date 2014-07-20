// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_PLANTS_MASTER_PLANT_H_
#define SRC_PLANTS_MASTER_PLANT_H_

#include "./plant.h"

class MasterPlant : public Plant {
public:
  // Constructor
  MasterPlant(World *world, clan::Canvas canvas,
              clan::Vec2f position, std::string name, PlantPlayer* player, bool menu);

  // Queries
  virtual bool is_master_plant() { return true; }
  bool CanBuild(clan::Vec2f position);
};

#endif  // SRC_PLANTS_MASTER_PLANT_H_

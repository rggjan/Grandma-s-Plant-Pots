// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_PLANTS_PLANT_H_
#define SRC_PLANTS_PLANT_H_

#include "./gameobject.h"
#include "./world.h"

class PlantPlayer;

class Plant : public GameObject {
  public:
    // Construction
    Plant(World *world, clan::Canvas canvas,
          clan::Vec2f position, std::string name, PlantPlayer* player);

    virtual inline int sugar_cost() = 0;

    virtual Plant *GetNewPlant(clan::Vec2f position, clan::Canvas canvas) = 0;

    // Operations
    virtual bool has_children() { return false; }
    virtual void DrawTmp(clan::Canvas canvas, float x, float y, float alpha = 1.0, float size=1.0, clan::Color color = clan::Color::white);
    virtual void DrawTmpChild(clan::Canvas canvas) {};
    virtual void Draw(clan::Canvas canvas, clan::Vec2f position);

    // Queries
    virtual inline bool CanBuild(clan::Vec2f position) {
      return world_->CanBuild(position);
    }

    bool free_space() {
      return eating_bug_ == NULL;
    }

    void add_eating_bug(Bug* bug) {
      eating_bug_ = bug;
    }

    void remove_bugs() {
      eating_bug_ = NULL;
    }

    PlantPlayer* player() {
      return player_;
    }

    virtual bool Update(int time_ms);

    virtual void UpdateAnimation(int time_ms) {}

  protected:
    PlantPlayer* player_;
    Bug* eating_bug_;

    double co2_collected_per_second_;
    double sun_collected_per_second_;
};

#endif  // SRC_PLANTS_PLANT_H_

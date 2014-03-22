// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_GAMEOBJECT_H_
#define SRC_GAMEOBJECT_H_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "world.h"

namespace clan {
class StringRef;
}

class GameObject {
  public:
    // Construction
	GameObject(World* world, clan::Canvas *canvas,
			   clan::Vec2f position, std::string name);
    virtual ~GameObject() {}

    // Attributes
    inline clan::Angle angle() {
      return sprite_.get_angle();
    }
    inline void set_angle(clan::Angle angle) {
      sprite_.set_angle(angle);
    }
    inline bool is_alive() {
      return energy_ > 0;
    }
    inline clan::Vec2f position() {
      return position_;
    }

    // Operations
    virtual void Draw(clan::Canvas *canvas, clan::Vec2f target);
    virtual bool Update(int time_ms);
    virtual double DecreaseEnergy(double amount);

  protected:
    // Functions
    virtual void UpdateAnimation(int time_ms);

    // Variables
    World *world_;

    double energy_;
    double dead_time_;
    clan::Vec2f position_;

    clan::Sprite sprite_;
    clan::Color dead_color_;
};

#endif  // SRC_GAMEOBJECT_H_

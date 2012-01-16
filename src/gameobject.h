// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_GAMEOBJECT_H_
#define SRC_GAMEOBJECT_H_

#include <ClanLib/display.h>

#include "./world.h"

class GameObject {
  public:
    // Construction
    GameObject(World* world, CL_GraphicContext *gc,
               CL_Vec2f position, CL_StringRef name);
    virtual ~GameObject() {}

    // Attributes
    inline CL_Angle angle() {
      return sprite_.get_angle();
    }
    inline void set_angle(CL_Angle angle) {
      sprite_.set_angle(angle);
    }
    inline bool is_alive() {
      return energy_ > 0;
    }
    inline CL_Vec2f position() {
      return position_;
    }

    // Operations
    virtual void Draw(CL_GraphicContext *gc, CL_Vec2f target);
    virtual bool Update(int time_ms);
    virtual double DecreaseEnergy(double amount);

  protected:
    // Functions
    virtual void UpdateAnimation(int time_ms);

    // Variables
    World *world_;

    double energy_;
    double dead_time_;
    CL_Vec2f position_;

    CL_Sprite sprite_;
    CL_Color dead_color_;
};

#endif  // SRC_GAMEOBJECT_H_

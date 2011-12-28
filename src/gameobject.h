// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_GAMEOBJECT_H_
#define SRC_GAMEOBJECT_H_

#include <ClanLib/display.h>

class World;

class GameObject {
  public:
    // Construction
    GameObject(World *world, CL_Vec2f position_);
    virtual ~GameObject() {}

    inline void set_angle(CL_Angle angle) {
      spriteImage->set_angle(angle);
    }
    inline CL_Angle angle() {
      return spriteImage->get_angle();
    }

    inline bool is_alive() {
      return energy_ > 0;
    }

    // Operations
    virtual void Draw(CL_GraphicContext *gc, CL_Vec2f target);
    virtual bool Update(int time_ms);

    double DecreaseEnergy(double amount);

    inline CL_Vec2f position() {
      return position_;
    }

    CL_Vec2f position_;

  protected:
    double energy_;

    CL_Sprite *spriteImage;
    World *world_;
    double dead_time_;
};

#endif  // SRC_GAMEOBJECT_H_

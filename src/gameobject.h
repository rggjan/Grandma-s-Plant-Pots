// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_GAMEOBJECT_H_
#define SRC_GAMEOBJECT_H_

#include <ClanLib/display.h>

class World;

class GameObject {
  public:
    // Construction
    GameObject(World *world, CL_Vec2f position_);
    explicit GameObject(World *world);
    virtual ~GameObject() {}

    inline void set_angle(CL_Angle angle) { spriteImage->set_angle(angle); }
    inline CL_Angle angle() { return spriteImage->get_angle(); }

    inline bool is_alive() { return alive_; }
    inline void set_dead() { alive_ = false; }

    // Operations
    virtual void Draw(CL_GraphicContext *gc, CL_Vec2f target);
    virtual bool Update(int time_ms);

    inline CL_Vec2f position() { return position_; }

    CL_Vec2f position_;

  protected:
    CL_Sprite *spriteImage;
    World *world_;
    bool alive_;
    double dead_time_;
};

#endif  // SRC_GAMEOBJECT_H_

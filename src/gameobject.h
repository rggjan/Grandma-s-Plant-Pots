// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_GAMEOBJECT_H_
#define SRC_GAMEOBJECT_H_

#include <ClanLib/core.h>
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

    // Operations
    virtual void Draw(CL_GraphicContext *gc, CL_Vec2f target);
    virtual bool update(int timeElapsed_ms);

    inline CL_Vec2f position() { return position_; }

    CL_Vec2f position_;

  protected:
    CL_Sprite *spriteImage;
    World *world;
};

#endif  // SRC_GAMEOBJECT_H_

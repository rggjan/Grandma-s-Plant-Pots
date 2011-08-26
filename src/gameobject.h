// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_GAMEOBJECT_H_
#define SRC_GAMEOBJECT_H_

#include <ClanLib/core.h>

class World;
class CL_GraphicContext;
class CL_Sprite;

class GameObject {
  public:
    // Construction
    GameObject(World *world, CL_Vec2f position_);
    GameObject(World *world);
    virtual ~GameObject() {}

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

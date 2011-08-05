// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_FLY_H_
#define SRC_FLY_H_

#include <ClanLib/core.h>

#include "./gameobject.h"

class CL_Sprite;

class Fly : public GameObject {
  // Construction
  public:
    explicit Fly(World *world);

  // Operations:
  public:

    void setPos(int x, int y);
    void setTargetPos(int x, int y);

    virtual void draw(int x, int y);
    virtual bool update(int timeElapsed_ms);

  // Implementation:
  private:
    CL_Vec2f direction;
    CL_Sprite *spriteDragonfly;

    float posX, posY;
    float destPosX, destPosY;
};

#endif  // SRC_FLY_H_

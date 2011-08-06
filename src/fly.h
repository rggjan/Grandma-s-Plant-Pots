// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_FLY_H_
#define SRC_FLY_H_

#include <ClanLib/core.h>

#include "./gameobject.h"

class CL_Sprite;
class CL_GraphicContext;

class Fly : public GameObject {
  // Construction
  public:
    Fly(World *world, CL_GraphicContext *gc);

  // Operations:
  public:

    void setPos(int x, int y);
    void setTargetPos(int x, int y);

    virtual void draw(int x, int y, CL_GraphicContext &gc);
    virtual bool update(int timeElapsed_ms);

  // Implementation:
  private:
    CL_Vec2f direction;
    CL_Sprite *spriteDragonfly;

    float posX, posY;
    float destPosX, destPosY;
};

#endif  // SRC_FLY_H_

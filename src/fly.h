// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_FLY_H_
#define SRC_FLY_H_

#include <ClanLib/core.h>

#include "./gameobject.h"
#include "./flower.h"

class CL_Sprite;
class CL_GraphicContext;

class Fly : public GameObject {
    // Construction
  public:
    Fly(World *world, CL_GraphicContext &gc, const CL_StringRef &name);

    // Operations:
  public:
    inline void set_position(CL_Vec2f position) {
      position_ = position;
    }

    inline void set_target_position(CL_Vec2f target_position) {
      target_position_ = target_position;
    }

    void SetTargetFlower(Flower *flower);

    virtual bool update(int timeElapsed_ms);

    // Implementation:
  private:
    CL_Vec2f direction;
    CL_Vec2f target_position_;
    Flower *target_flower_;
};

#endif  // SRC_FLY_H_

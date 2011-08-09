// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_FLOWER_H_
#define SRC_FLOWER_H_

#include <ClanLib/core.h>

#include "./gameobject.h"

class Flower : public GameObject {
  // Construction
  public:
    Flower(World *world, CL_GraphicContext *gc, float posX, float posY);
    virtual void draw(CL_GraphicContext *gc, int x, int y);

  private:
    CL_Sprite *selectedImage;
};

#endif  // SRC_FLOWER_H_

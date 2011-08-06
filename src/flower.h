#ifndef SRC_FLOWER_H_
#define SRC_FLOWER_H_

#include <ClanLib/core.h>

#include "./gameobject.h"

class Flower : public GameObject {
  // Construction
  public:
    Flower(World *world, CL_GraphicContext &gc, float posX, float posY);
};

#endif  // 

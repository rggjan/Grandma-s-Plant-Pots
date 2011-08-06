#ifndef SRC_LEAF_H_
#define SRC_LEAF_H_

#include <ClanLib/core.h>

#include "./gameobject.h"

class Leaf : public GameObject {
  // Construction
  public:
    Leaf(World *world, CL_GraphicContext &gc, float posX, float posY);
};

#endif  // 

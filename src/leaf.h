// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_LEAF_H_
#define SRC_LEAF_H_

#include <ClanLib/core.h>

#include "./gameobject.h"

class Leaf : public GameObject {
  // Construction
  public:
    Leaf(World *world, CL_GraphicContext &gc, const CL_StringRef &name,
    float posX, float posY);
};

#endif  // SRC_LEAF_H_

// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_LEAF_H_
#define SRC_LEAF_H_

#include <ClanLib/core.h>

#include "./gameobject.h"

class Leaf : public GameObject {
    // Construction
  public:
    Leaf(World *world, CL_GraphicContext *gc, const CL_StringRef &name,
         CL_Vec2f position);

    void DrawRed(CL_GraphicContext *gc, CL_Vec2f position);
    void DrawGreen(CL_GraphicContext *gc, CL_Vec2f position);

    void SetAngle(CL_Angle angle);
};

#endif  // SRC_LEAF_H_

#ifndef SRC_LEAF_H_
#define SRC_LEAF_H_

#include <ClanLib/core.h>

#include "./gameobject.h"

class CL_Sprite;

class Leaf : public GameObject {
  // Construction
  public:
    explicit Leaf(World *world, float posX, float posY);

  // Operations:
  public:
    virtual void draw(int x, int y);
    virtual bool update(int timeElapsed_ms);

  // Implementation:
  private:
    CL_Vec2f direction;
    CL_Sprite *leafGraphic;

    float posX, posY;
};

#endif  // 

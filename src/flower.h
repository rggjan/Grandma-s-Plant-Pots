#ifndef SRC_FLOWER_H_
#define SRC_FLOWER_H_

#include <ClanLib/core.h>

#include "./gameobject.h"

class CL_Sprite;

class Flower : public GameObject {
  // Construction
  public:
    explicit Flower(World *world, float posX, float posY);

  // Operations:
  public:
    virtual void draw(int x, int y);
    virtual bool update(int timeElapsed_ms);

  // Implementation:
  private:
    CL_Vec2f direction;
    CL_Sprite *flowerGraphic;

    float posX, posY;
};

#endif  // 

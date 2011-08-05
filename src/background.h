// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_BACKGROUND_H_
#define SRC_BACKGROUND_H_

#include "./gameobject.h"

class CL_Sprite;
class CL_SoundBuffer;

class Background : public GameObject {
  // Construction
  public:
    explicit Background(World *world);
    ~Background();

  // Attributes
  public:

  // Operations
  public:
    void setPos(int x, int y);

    virtual void draw(int x, int y);
    virtual bool update(int timeElapsed_ms);

  // Implementation:
  private:
    CL_Sprite *sprite;

    float posX;
    float posY;
};

#endif  // SRC_BACKGROUND_H_

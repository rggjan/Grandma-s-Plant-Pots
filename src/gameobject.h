// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_GAMEOBJECT_H_
#define SRC_GAMEOBJECT_H_

class World;
class CL_GraphicContext;
class CL_Sprite;

class GameObject {
  public:
    // Construction
    explicit GameObject(World *world);
    virtual ~GameObject() {}

    // Operations
    virtual void draw(CL_GraphicContext *gc, int x, int y);
    virtual bool update(int timeElapsed_ms);

  protected:
    CL_Sprite *spriteImage;
    World *world;
    float posX, posY;
};

#endif  // SRC_GAMEOBJECT_H_

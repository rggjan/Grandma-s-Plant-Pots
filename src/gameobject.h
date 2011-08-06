// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_GAMEOBJECT_H_
#define SRC_GAMEOBJECT_H_

class World;
class CL_CollisionOutline;
class CL_GraphicContext;

class GameObject {
  // Construction
  public:
    explicit GameObject(World *world) {
      this->world = world;
    }

    virtual ~GameObject() {}

  // Attributes
  public:

  // Operations
  public:
    virtual void draw(int x, int y, CL_GraphicContext &gc) {}
    virtual bool update(int timeElapsed_ms) {
      return false;
    }
    virtual bool hitCheck(CL_CollisionOutline *outline) {
      return false;
    }

  protected:
    World *world;
};

#endif  // SRC_GAMEOBJECT_H_

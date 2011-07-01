#ifndef SRC_GAMEOBJECT_H_
#define SRC_GAMEOBJECT_H_

class World;
class CL_CollisionOutline;

class GameObject
{
// Construction
public:
	GameObject(World *world) { this->world = world; }

// Attributes
public:
	
// Operations
public:
	virtual void draw(int x, int y) {};
	virtual bool update(int timeElapsed_ms) { return false; }
	virtual bool hitCheck(CL_CollisionOutline *outline) { return false; }
	
protected:
	World *world;
};

#endif // SRC_GAMEOBJECT_H_

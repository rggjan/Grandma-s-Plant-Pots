#ifndef SRC_BUILDING_H
#define SRC_BUILDING_H

#include "gameobject.h"

class CL_Sprite;
class CL_SoundBuffer;

class Building : public GameObject
{
// Enums
public:
	enum BuildingType
	{
		HELI_PAD
	};

// Construction
public:
	Building(BuildingType building_type, World *world);
	~Building();
	
// Attributes
public:
	
// Operations
public:
	void setPos(int x, int y);
	void setAngle(float angle);
	
	virtual void draw(int x, int y);
	virtual bool update(int timeElapsed_ms);
	virtual bool hitCheck(CL_CollisionOutline *outline);

// Implementation:
private:
	CL_Sprite *sprite;
	CL_CollisionOutline *collisionBuilding;
	
	float angle;	
	float posX;
	float posY;
	
	bool exploding;
};

#endif // SRC_BUILDING_H

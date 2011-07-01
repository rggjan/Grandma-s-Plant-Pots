#pragma once

#include "gameobject.h"

class CL_Sprite;
class CL_SoundBuffer;
class World;

class Fly : public GameObject
{
// Construction
public:
	Fly(World *world);

// Operations:
public:
	
	void setPos(int x, int y);
	
  void setTargetPos(int x, int y);

	virtual void draw(int x, int y);
	virtual bool update(int timeElapsed_ms);

// Implementation:
private:
  CL_Vec2f direction;
  CL_Vec2f target_direction;

	CL_Sprite *spriteDragonfly;


	float turretAngle;
	float destTurretAngle, deltaTurretAngle;

	float posX, posY;
	float destPosX, destPosY;
	float deltaPosX, deltaPosY;
};
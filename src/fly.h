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

// Attributes:
public:
	bool isSelected() const;

// Operations:
public:
	void setTurretAngle(float angle);
	void setDestTurretAngle(float angle);

	void setPos(int x, int y);
	void setTargetPos(int x, int y);

  void setTurretTargetPos(int x, int y);

	virtual void draw(int x, int y);
	virtual bool update(int timeElapsed_ms);

// Implementation:
private:
	CL_Sprite *spriteTurretShooting;

	float bodyAngle;
	float destBodyAngle, deltaBodyAngle;

	float turretAngle;
	float destTurretAngle, deltaTurretAngle;

	float posX, posY;
	float destPosX, destPosY;
	float deltaPosX, deltaPosY;

	float bodyTurnSpeed;
	float turretTurnSpeed;
	float moveSpeed;

	bool selected;

	bool isFiring;
	bool reverse;
};

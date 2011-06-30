#ifndef SRC_CROSS_H
#define SRC_CROSS_H

#include "gameobject.h"

class CL_Sprite;
class CL_SoundBuffer;

class Cross : public GameObject
{
// Construction
public:
	Cross(World *world);
	~Cross();

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

#endif // SRC_CROSS_H

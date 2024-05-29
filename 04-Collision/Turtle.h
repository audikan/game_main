#pragma once
#include "GameObject.h"
#include "Goomba.h"
#include "Brick.h"
#define TURTLE_WALKING_SPEED 0.05f
#define TURTLE_GRAVITY 0.002f
#define TURTLE_BBOX_WIDTH 18.0f
#define TURTLE_BBOX_HEIGHT 25.0f
#define TURTLE_BBOX_HEIGHT_DIE 15.0f

#define TURTLE_DIE_TIMEOUT 10000

#define TURTLE_STATE_WALKING_LEFT 850
#define TURTLE_STATE_WALKING_RIGHT 860
#define TURTLE_STATE_SPIN_RIGHT 880
#define TURTLE_STATE_SPIN_LEFT 890
#define TURTLE_STATE_DIE 900

#define ID_ANI_TURTLE_WALKING_LEFT 9000
#define ID_ANI_TURTLE_WALKING_RIGHT 9001
#define ID_ANI_TURTLE_DIE 9002
#define ID_ANI_TURTLE_SPIN 9003

class CTurtle : public CGameObject
{
protected:
	float ax;
	float ay;

	ULONGLONG die_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CTurtle(float x, float y);
	virtual void SetState(int state);
};
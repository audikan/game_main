#pragma once
#include "GameObject.h"

#define GOOMBA_GRAVITY 0.0002f
#define GOOMBA_WALKING_SPEED 0.05f


#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 14
#define GOOMBA_BBOX_HEIGHT_DIE 7

#define GOOMBA_DIE_TIMEOUT 500

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200

#define GOOMBA_STATE_FLY 300
#define GOOMBA_STATE_FLY_WALK 301

#define ID_ANI_GOOMBA_WALKING 5000
#define ID_ANI_GOOMBA_DIE 5001
#define ID_ANI_GOOMBA_FLY 5012
#define ID_ANI_GOOMBA_FLY_WALK 5011

#define GOOMBA_FLY_WALK_BBOX_WIDTH 19
#define GOOMBA_FLY_WALK_BBOX_HEIGHT 18
#define GOOMBA_FLY_BBOX_HEIGHT 23
#define GOOMBA_FLY_BBOX_WIDTH 19

class CGoomba : public CGameObject
{
protected:
	float ax;				
	float ay; 

	ULONGLONG die_start, time_fly;

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public: 	
	CGoomba(float x, float y);
	CGoomba(float x, float y, int fly);
	virtual void SetState(int state);
};
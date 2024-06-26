#pragma once
#include "GameObject.h"

#define MUSHROOM_GRAVITY 0.002f
#define MUSHROOM_WALKING_SPEED 0.1f


#define MUSHROOM_BBOX_WIDTH 18
#define MUSHROOM_BBOX_HEIGHT 18

#define ID_ANI_MUSHROOM 7201

class CMushroom : public CGameObject
{
protected:
	float ax;
	float ay;
	float y_temp;
	bool inBox;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CMushroom(float x, float y);
	void setInBox() { inBox = true; };
	virtual void SetState(int state);
};
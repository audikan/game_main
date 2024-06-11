#pragma once
#include "GameObject.h"

#define BULLET_SPEED 0.1f
#define BULLET_BBOX_WIDTH 9
#define ID_ANI_BULLET_FLOWER 1411

class CBullet : public CGameObject
{
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual int IsBlocking() { return 0; }
	CBullet(float x, float y) : CGameObject(x, y) {}
};
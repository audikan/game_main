#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_MUSHROOM 13000
#define MUSHROOM_WIDTH 16
#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16

//----------------------
#define MUSHROOM_STATE_ACTIVE 700
#define ID_ANI_MUSHROOM_ACTIVE 7001


class CMushroom: public CGameObject {
protected:
	ULONGLONG isActivated;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	CMushroom(float x, float y) : CGameObject(x, y) {}
	virtual void SetState(int state);
};
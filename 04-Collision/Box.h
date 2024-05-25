#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BOX 12000
#define BOX_WIDTH 16
#define BOX_BBOX_WIDTH 16
#define BOX_BBOX_HEIGHT 16

#define BOX_STATE_ACTIVE 700
#define ID_ANI_BOX_ACTIVE 7001
#define BOX_STATE_UNACTIVE 800

class CBox : public CGameObject {
protected:
	bool isActivated;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	CBox(float x, float y) : CGameObject(x, y) {
		isActivated = false;
		SetState(BOX_STATE_UNACTIVE);
	}
	virtual void SetState(int state) {
		CGameObject::SetState(state);
	};
};
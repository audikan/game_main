#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_MUSH 12000
#define MUSH_WIDTH 16
#define MUSH_BMUSH_WIDTH 16
#define MUSH_BMUSH_HEIGHT 16

#define MUSH_STATE_ACTIVE 700
#define ID_ANI_MUSH_ACTIVE 7001
#define MUSH_STATE_UNACTIVE 800

class CBox : public CGameObject {
protected:
	bool isActivated;
	float y_temp;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	CBox(float x, float y) : CGameObject(x, y) {
			isActivated = false;
		SetState(MUSH_STATE_UNACTIVE);
	}
	virtual void SetState(int state) {
		CGameObject::SetState(state);
	};
};
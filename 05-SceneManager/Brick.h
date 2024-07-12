#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BRICK 10000
#define ID_ANI_BRICK_BOOM 10002

#define STATE_BRICK_BOOM 111
#define STATE_BRICK 110

#define BRICK_WIDTH 15
#define BRICK_BBOX_WIDTH 15
#define BRICK_BBOX_HEIGHT 15

class CBrick : public CGameObject {
public:
	ULONGLONG time_boom;
	CBrick(float x, float y) : CGameObject(x, y) {
		state = STATE_BRICK;
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetState(int state) {
		if (state = STATE_BRICK_BOOM) {
			time_boom = GetTickCount64();
		}
		CGameObject::SetState(state);
	}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};
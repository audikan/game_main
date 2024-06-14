#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "inBlackBox.h"

#define ID_ANI_BLACK_BOX_MUSHROOM 7301
#define ID_ANI_BLACK_BOX_STAR 7302
#define ID_ANI_BLACK_BOX_FLOWER 7303
#define ID_ANI_BLACK_BOX_ACTIVE 7305

#define STATE_BLACK_BOX_MUSHROOM 7401
#define STATE_BLACK_BOX_STAR 7402
#define STATE_BLACK_BOX_FLOWER 7403

#define STATE_BLACK_BOX_ACTIVE 7405

#define	BLACK_BOX_WIDTH 25



class CBlackBox : public CGameObject {
public:
	ULONGLONG time_trans;
	CBlackBox(float x, float y) : CGameObject(x, y) {
		SetState(STATE_BLACK_BOX_STAR);
	}
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void Render();
	virtual void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	void createInBox(int state);
};

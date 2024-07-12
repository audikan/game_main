#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BOX 7001
#define ID_ANI_BOX_ACTIVE 7002
#define STATE_BOX_UNACTIVE 7101
#define STATE_BOX_ACTIVE 7201
#define	BOX_WIDTH 15
#define BOX_BBOX_WIDTH 15
#define BOX_BBOX_HEIGHT 15

class CBox : public CGameObject {
protected:
	int coins;
	int type; // 1 = coin, 2 = mushroom
	float y_temp;
public:
	CBox(float x, float y) : CGameObject(x, y) {
		y_temp = y;
		SetState(STATE_BOX_UNACTIVE);
		type = 2;
	}
	CBox(float x, float y, int num_coins) : CGameObject(x, y) {
		coins = num_coins;
		y_temp = y;
		SetState(STATE_BOX_UNACTIVE);
		type = 1;
	}
	virtual int getCoins() {
		return coins;
	}
	virtual int getType() {
		return type;
	}
	virtual void updateCoins() {
		coins--;
		y -= 5.0f;
	}
	void createCoin();
	void createMushroom(int m);
	void Render();
	virtual void SetState(int state) {
		CGameObject::SetState(state);
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 1; }
};
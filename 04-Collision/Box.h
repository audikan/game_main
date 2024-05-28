#pragma once
#include "Coin.h"
#include "Game.h"
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Mushroom.h"
#define ID_ANI_BOX 12000
#define BOX_WIDTH 16
#define BOX_BBOX_WIDTH 16
#define BOX_BBOX_HEIGHT 16

#define BOX_STATE_ACTIVE 700
#define ID_ANI_BOX_ACTIVE 7001
#define BOX_STATE_UNACTIVE 800

#define BOX_MUSHROOM_UNACTIVE 7101
#define BOX_COIN_UNACTIVE 7102

class CBox : public CGameObject {
protected:
	bool isActivated;
	float y_temp;
	int coins;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	CBox(float x, float y, int m, int n) : CGameObject(x, y) {
		coins = m;
		y_temp = y;
		isActivated = false;
		SetState(BOX_COIN_UNACTIVE);
	}
	CBox(float x, float y, bool mush) : CGameObject(x, y) {
		coins = 0;
		y_temp = y;
		isActivated = false;
		SetState(BOX_MUSHROOM_UNACTIVE);
	}
	void CreateMushroom() {
		CMushroom* mush = new CMushroom(x, y - BOX_BBOX_WIDTH);
		mush->setInBox();
		CGame::GetInstance()->AddObject(mush);
	};
	void CreateCoin() {
		CCoin* c = new CCoin(x, y- BOX_BBOX_WIDTH);
		c->setInBox();
		CGame::GetInstance()->AddObject(c);
	};
	virtual int getCoins() {
		return coins;
	}
	virtual void updateCoins() {
		coins--;
		y -= 5.0f;
	}
	virtual void SetState(int state) {
		CGameObject::SetState(state);
	};
};
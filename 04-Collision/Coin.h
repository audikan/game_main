#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_COIN 11000

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

class CCoin : public CGameObject {
protected:
	bool isInBox;
	float y_temp;
public:
	CCoin(float x, float y) : CGameObject(x, y) {}
	CCoin(float x, float y, bool isInBox) : CGameObject(x, y) {
		isInBox = isInBox;
		y_temp = y;
	}
	void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	
};
#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define FLOWER_SPEED 0.05f

#define FLOWER_BBOX_WIDTH 16
#define FLOWER_BBOX_HEIGHT 23

#define FLOWER_STATE_ATTACK 100
#define FLOWER_STATE_NOL 200

#define ID_ANI_FLOWER_ATTACK 16123
#define ID_ANI_FLOWER_NOL 16001

//================
class CFlower : public CGameObject {
protected:
	bool isInPipeline;
	float y_temp;
public:
	CFlower(float x, float y) : CGameObject(x, y) {
		isInPipeline = false;
		y_temp = y;
		SetState(FLOWER_STATE_NOL);
	}
	void setInBox() {
		isInPipeline = true;
		y_temp = y;
	}
	virtual void SetState(int state);
	void Render();
	float IsMarioNearby();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }

};

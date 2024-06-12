#pragma once
#include "GameObject.h"

#define ID_ANI_FLOWER 7503
#define ID_ANI_FLOWER_FIRE 7504
#define FLOWER_STATE_FIRE 151
#define FLOWER_STATE_NOL 150
#define FLOWER_STATE_NOL_LAN 152

#define FLOWER_WIDTH 16
#define FLOWER_HEIGHT 23
#define M_PI 3.14159265358979323846
class CFlower : public CGameObject
{
protected:
	ULONGLONG fire_time, lan_time;
	float y_temp;
public:
	CFlower(float x, float y) : CGameObject(x, y) {
		y_temp = y;
		SetState(FLOWER_STATE_NOL);
	}
	void Render();
	void fireMario();
	void SetState(int state);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};
//============================================================

#pragma once
#include "GameObject.h"

#define ID_ANI_FLOWER_LEFT 7511
#define ID_ANI_FLOWER_RIGHT 7513
#define ID_ANI_FLOWER_FIRE_LEFT 7512
#define ID_ANI_FLOWER_FIRE_RIGHT 7514

#define FLOWER_STATE_FIRE_LEFT 151
#define FLOWER_STATE_FIRE_RIGHT 152

#define FLOWER_STATE_NOL_LEFT 161
#define FLOWER_STATE_NOL_RIGHT 162

#define FLOWER_STATE_NOL_LAN_LEFT 171
#define FLOWER_STATE_NOL_LAN_RIGHT 172

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
		SetState(FLOWER_STATE_NOL_LEFT);
	}
	void Render();
	void fireMario();
	void SetState(int state);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};
//============================================================

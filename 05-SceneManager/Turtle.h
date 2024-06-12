#pragma once
#include "GameObject.h"

#define TURTLE_GRAVITY 0.002f
#define TURTLE_WALKING_SPEED 0.05f


#define TURTLE_BBOX_WIDTH 15
#define TURTLE_BBOX_HEIGHT 26

#define TURTLE_BBOX_WIDTH_DIE 17
#define TURTLE_BBOX_HEIGHT_DIE 15

#define TURTLE_BBOX_SPIN 15


#define TURTLE_STATE_WALK_LEFT 7601
#define TURTLE_STATE_WALK_RIGHT 7611
#define TURTLE_STATE_DIE 7602
#define TURTLE_STATE_SPIN_LEFT 7603
#define TURTLE_STATE_SPIN_RIGHT 7604


#define ID_ANI_TURTLE_WALK_LEFT 76001
#define ID_ANI_TURTLE_WALK_RIGHT 76002

#define ID_ANI_TURTLE_DIE 76003
#define ID_ANI_TURTLE_SPIN 76004

class CTurtle : public CGameObject
{
protected:
	float dis, x_temp;
	float ax;
	float ay;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CTurtle(float x, float y, float dis);
	virtual void SetState(int state);
	void tang_y() {
		y -= 5.0f;
	}
	float get_x() {
		return x;
	}
};
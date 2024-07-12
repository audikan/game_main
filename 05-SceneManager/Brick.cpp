#include "Brick.h"

void CBrick::Render()
{
	int ani = ID_ANI_BRICK + 1;
	if (state == STATE_BRICK_BOOM) {
		ani = ID_ANI_BRICK_BOOM;
	}
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ani)->Render(x, y);
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x - BRICK_BBOX_WIDTH/2;
	t = y - BRICK_BBOX_HEIGHT/2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}

void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if ((state == STATE_BRICK_BOOM) && (GetTickCount64() - time_boom > 300))
	{
		isDeleted = true;
		return;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

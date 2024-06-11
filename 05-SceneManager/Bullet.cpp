#include "Bullet.h"

void CBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - BULLET_BBOX_WIDTH / 2;
	top = y - BULLET_BBOX_WIDTH / 2;
	right = left + BULLET_BBOX_WIDTH;
	bottom = top + BULLET_BBOX_WIDTH;
}

void CBullet::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(14011)->Render(x, y);
}
void CBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	x -= BULLET_SPEED*dt;
	y += BULLET_SPEED*dt;
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
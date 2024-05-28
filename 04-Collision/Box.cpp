#include "Box.h"
#include "Mario.h"
#include "Coin.h"

void CBox::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BOX_BBOX_WIDTH / 2;
	t = y - BOX_BBOX_HEIGHT / 2;
	r = l + BOX_BBOX_WIDTH;
	b = t + BOX_BBOX_HEIGHT;
}
//------------------------
void CBox::OnCollisionWith(LPCOLLISIONEVENT e)
{
}

void CBox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (y_temp > y) {
		y += 1.0f;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CBox::Render()
{
	int aniId = ID_ANI_BOX;
	if (state == BOX_STATE_ACTIVE)
	{
		aniId = ID_ANI_BOX_ACTIVE;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}
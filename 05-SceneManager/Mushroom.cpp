#include "Mushroom.h"

CMushroom::CMushroom(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	x_temp = x;
	type = 1;
	this->ay = MUSHROOM_GRAVITY;
	this->vx = -MUSHROOM_WALKING_SPEED;
}

CMushroom::CMushroom(float x, float y, int type) :CGameObject(x, y)
{
	this->ax = 0;
	x_temp = x + 26.0f;
	this->type = type;
	switch (type)
	{
	case 1:
		this->ay = MUSHROOM_GRAVITY;
		this->vx = -MUSHROOM_WALKING_SPEED;
		y_temp = y;
		break;
	case 2:
		this->ay = MUSHROOM_GRAVITY / 50;
		if (x <= x_temp) this->vx = MUSHROOM_WALKING_SPEED;
		if (x - x_temp >= 40.0f) this->vx = -MUSHROOM_WALKING_SPEED;
		y_temp = y - 16.0f;
		break;
	default:
		break;
	}
		

}
void CMushroom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - MUSHROOM_BBOX_WIDTH / 2;
	top = y - MUSHROOM_BBOX_HEIGHT / 2;
	right = left + MUSHROOM_BBOX_WIDTH;
	bottom = top + MUSHROOM_BBOX_HEIGHT - 2.0f;
}

void CMushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (type == 2) return;
	if (!e->obj->IsBlocking()) return;
	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (inBox) {
		if (y > y_temp - 18.0f) {
			y -= 1.0f;
			vy = 0;
			ay = 0;
			vx = 0;
		}
		else {
			inBox = false;
		}
	}
	if (!inBox) {
		switch (type)
		{
		case 1:
			this->ay = MUSHROOM_GRAVITY;
			this->vx = -MUSHROOM_WALKING_SPEED;
			break;
		case 2:
			vy = 0;
			this->ay = MUSHROOM_GRAVITY / 7;
			if (x <= x_temp) this->vx = MUSHROOM_WALKING_SPEED;
			if (x - x_temp > 90.0f) this->vx = -MUSHROOM_WALKING_SPEED;
			break;
		default:
			break;
		}
	}
	vy += ay * dt;
	vx += ax * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CMushroom::Render()
{
	int aniId = ID_ANI_MUSHROOM;

	if (type == 2) {
		aniId = ID_ANI_LA;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CMushroom::SetState(int state) {}

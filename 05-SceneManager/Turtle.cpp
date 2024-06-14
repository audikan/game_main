#include "Turtle.h"
#include "retangle.h"
#include "Goomba.h"
#include "Box.h"
#include "Mario.h"
#include "ObjBlock.h"
#include "Platform.h"
#include "Brick.h"

CTurtle::CTurtle(float x, float y, float dis) : CGameObject(x, y)
{
	x_temp = x;
	this->dis = dis;
	this->ax = 0;
	this->ay = TURTLE_GRAVITY;
	SetState(TURTLE_STATE_WALK_LEFT);
}

void CTurtle::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == TURTLE_STATE_WALK_LEFT || state == TURTLE_STATE_WALK_RIGHT)
	{
		left = x - TURTLE_BBOX_WIDTH/2;
		top = y - TURTLE_BBOX_HEIGHT/2;
		right = left + TURTLE_BBOX_WIDTH;
		bottom = top + TURTLE_BBOX_HEIGHT;
	}
	if(state == TURTLE_STATE_DIE)
	{
		left = x - TURTLE_BBOX_WIDTH_DIE/2;
		top = y - TURTLE_BBOX_HEIGHT_DIE/2;
		right = left + TURTLE_BBOX_WIDTH_DIE;
		bottom = top + TURTLE_BBOX_HEIGHT_DIE;
	}
	if (state == TURTLE_STATE_SPIN_LEFT || state == TURTLE_STATE_SPIN_RIGHT) {
		left = x - TURTLE_BBOX_SPIN/2;
		top = y - TURTLE_BBOX_SPIN/2;
		right = left + TURTLE_BBOX_SPIN;
		bottom = top + TURTLE_BBOX_SPIN;
	}
}

void CTurtle::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CTurtle::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny < 0) {
		vy = 0;
	}
	if (e->nx != 0) {
		if (state == TURTLE_STATE_SPIN_LEFT || state == TURTLE_STATE_SPIN_RIGHT) {
			if (dynamic_cast<CGoomba*>(e->obj)) {
				e->obj->SetState(GOOMBA_STATE_DIE);
			}
			if (dynamic_cast<CObjBlock*>(e->obj)) {
				if (e->nx > 0) {
					SetState(TURTLE_STATE_SPIN_RIGHT);
				}
				else if (e->nx < 0) {
					SetState(TURTLE_STATE_SPIN_LEFT);
				}
			}
			if (dynamic_cast<CBrick*>(e->obj)) {
				if (e->nx > 0) {
					SetState(TURTLE_STATE_SPIN_RIGHT);
				}
				else if (e->nx < 0) {
					SetState(TURTLE_STATE_SPIN_LEFT);
				}
			}
			if (dynamic_cast<CPlatform*>(e->obj)) {
				if (e->nx > 0) {
					SetState(TURTLE_STATE_SPIN_RIGHT);
				}
				else if (e->nx < 0) {
					SetState(TURTLE_STATE_SPIN_LEFT);
				}
			}
			if (dynamic_cast<CBox*>(e->obj)) {
				CBox* box = dynamic_cast<CBox*>(e->obj);
				box->createMushroom();
				box->SetState(STATE_BOX_ACTIVE);
				if (e->nx > 0) {
					SetState(TURTLE_STATE_SPIN_RIGHT);
				}
				else if (e->nx < 0) {
					SetState(TURTLE_STATE_SPIN_LEFT);
				}
			}
		}
		else if (state == TURTLE_STATE_WALK_LEFT || state == TURTLE_STATE_WALK_RIGHT) {
			if (dynamic_cast<CGoomba*>(e->obj)) return;
			if (!dynamic_cast<CMario*>(e->obj)) {
				if (e->nx > 0) {
					SetState(TURTLE_STATE_WALK_RIGHT);
				}
				else if (e->nx < 0) {
					SetState(TURTLE_STATE_WALK_LEFT);
				}
			}
		}
	}
}

void CTurtle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (dis > 0) {
		if (x_temp - x > dis) {
			SetState(TURTLE_STATE_WALK_RIGHT);
		}
		if (x - x_temp > dis) {
			SetState(TURTLE_STATE_WALK_LEFT);
		}
	}
	vy += ay * dt;
	vx += ax * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CTurtle::Render()
{
	int aniId = 0;
	switch (state)
	{
		case TURTLE_STATE_WALK_LEFT:
		{
			aniId = ID_ANI_TURTLE_WALK_LEFT;
			break;
		}
		case TURTLE_STATE_WALK_RIGHT:
		{
			aniId = ID_ANI_TURTLE_WALK_RIGHT;
			break;
		}
		case TURTLE_STATE_DIE:
		{
			aniId = ID_ANI_TURTLE_DIE;
			dis = 0;
			break;
		}
		case TURTLE_STATE_SPIN_LEFT:
		{
			aniId = ID_ANI_TURTLE_SPIN;
			break;
		}
		case TURTLE_STATE_SPIN_RIGHT:
		{
			aniId = ID_ANI_TURTLE_SPIN;
			break;
		}
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CTurtle::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case TURTLE_STATE_WALK_RIGHT:
		vx = TURTLE_WALKING_SPEED;
		break;
	case TURTLE_STATE_WALK_LEFT:
		vx = -TURTLE_WALKING_SPEED;
		break;
	case TURTLE_STATE_DIE:
		vx = 0;
		vy = 0;
		ay = 0;
		y += 5.0f;
		break;
	case TURTLE_STATE_SPIN_LEFT:
		vx = -TURTLE_WALKING_SPEED*4;
		this->ay = TURTLE_GRAVITY;
		break;
	case TURTLE_STATE_SPIN_RIGHT:
		vx = TURTLE_WALKING_SPEED * 4;
		this->ay = TURTLE_GRAVITY;
		break;
	}
}

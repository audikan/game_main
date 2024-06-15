#include "Goomba.h"
#include "Mario.h"
#include "PlayScene.h"

CGoomba::CGoomba(float x, float y):CGameObject(x, y)
{
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
	die_start = -1;
	SetState(GOOMBA_STATE_WALKING);
	act = 0;
	vx = 0;
}
CGoomba::CGoomba(float x, float y, int i) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
	die_start = -1;
	SetState(GOOMBA_STATE_FLY_WALK);
	act = 0;
	vx = 0;
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (state == GOOMBA_STATE_DIE)
	{
		left = x - GOOMBA_BBOX_WIDTH/2;
		top = y - GOOMBA_BBOX_HEIGHT_DIE/2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT_DIE;
	}
	if (state == GOOMBA_STATE_FLY)
	{
		left = x - GOOMBA_FLY_BBOX_WIDTH / 2;
		top = y - GOOMBA_FLY_BBOX_HEIGHT / 2;
		right = left + GOOMBA_FLY_BBOX_WIDTH;
		bottom = top + GOOMBA_FLY_BBOX_HEIGHT;
	}
	if(state == GOOMBA_STATE_FLY_WALK)
	{
		left = x - GOOMBA_FLY_WALK_BBOX_WIDTH / 2;
		top = y - GOOMBA_FLY_WALK_BBOX_HEIGHT / 2;
		right = left + GOOMBA_FLY_WALK_BBOX_WIDTH;
		bottom = top + GOOMBA_FLY_WALK_BBOX_HEIGHT;
	}
	else
	{ 
		left = x - GOOMBA_BBOX_WIDTH/2;
		top = y - GOOMBA_BBOX_HEIGHT/2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT;
	}
}

void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return; 
	if (dynamic_cast<CGoomba*>(e->obj)) return; 

	if (e->ny != 0 )
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	float mario_x, mario_y, x_dis, y_dis;
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	mario->GetPosition(mario_x, mario_y);
	if (abs(x - mario_x) <= 170 && act == 0) {
		act = 1;
		vx = -GOOMBA_WALKING_SPEED;
	}
	if ((state == GOOMBA_STATE_FLY_WALK) && (GetTickCount64() - time_fly > GOOMBA_DIE_TIMEOUT)) {
		SetState(GOOMBA_STATE_FLY);
		vy = -GOOMBA_WALKING_SPEED * 3;
	}
	if ((state == GOOMBA_STATE_FLY) && (GetTickCount64() - time_fly > GOOMBA_DIE_TIMEOUT * 8))
		SetState(GOOMBA_STATE_FLY_WALK);

	if ((state == GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}
	vy += ay * dt;
	vx += ax * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CGoomba::Render()
{
	int aniId = ID_ANI_GOOMBA_WALKING;
	if (state == GOOMBA_STATE_DIE) 
	{
		aniId = ID_ANI_GOOMBA_DIE;
	}
	if (state == GOOMBA_STATE_FLY)
	{
		aniId = ID_ANI_GOOMBA_FLY;
	}
	if (state == GOOMBA_STATE_FLY_WALK)
	{
		aniId = ID_ANI_GOOMBA_FLY_WALK;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x,y);
	RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			die_start = GetTickCount64();
			y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE)/2;
			vx = 0;
			vy = 0;
			ay = 0; 
			break;
		case GOOMBA_STATE_WALKING: 
			vx = -GOOMBA_WALKING_SPEED;
			break;
		case GOOMBA_STATE_FLY_WALK:
			vx = -GOOMBA_WALKING_SPEED;
			time_fly = GetTickCount64();
			break;
		case GOOMBA_STATE_FLY:
			time_fly = GetTickCount64();
			vx = -GOOMBA_WALKING_SPEED;
			break;
	}
}

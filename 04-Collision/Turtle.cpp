#include "Turtle.h"
#include "Mario.h"
#include "Goomba.h"
#include "Brick.h"

CTurtle::CTurtle(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = TURTLE_GRAVITY;
	die_start = -1;
	SetState(TURTLE_STATE_WALKING_LEFT);
}

void CTurtle::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == TURTLE_STATE_DIE)
	{
		left = x - TURTLE_BBOX_WIDTH / 2;
		top = y - TURTLE_BBOX_HEIGHT_DIE / 2;
		right = left + TURTLE_BBOX_WIDTH;
		bottom = top + TURTLE_BBOX_HEIGHT_DIE;
	}
	else if(state != TURTLE_STATE_DIE && state != TURTLE_STATE_SPIN_LEFT && state != TURTLE_STATE_SPIN_RIGHT)
	{
		left = x - TURTLE_BBOX_WIDTH / 2;
		top = y - TURTLE_BBOX_HEIGHT / 2;
		right = left + TURTLE_BBOX_WIDTH;
		bottom = top + TURTLE_BBOX_HEIGHT;
	}
	else if (state == TURTLE_STATE_SPIN_LEFT || state == TURTLE_STATE_SPIN_RIGHT)
	{
		left = x - TURTLE_BBOX_WIDTH / 2;
		top = y - TURTLE_BBOX_HEIGHT_DIE / 2 - 2.0f;
		right = left + TURTLE_BBOX_WIDTH;
		bottom = top + TURTLE_BBOX_HEIGHT_DIE;
	}
}

void CTurtle::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

//====================================================
void CTurtle::OnCollisionWith(LPCOLLISIONEVENT e) {
    if (e->ny < 0) {
        vy = 0;
    } 
	else if (e->nx != 0) {
        if (state == TURTLE_STATE_SPIN_LEFT || state == TURTLE_STATE_SPIN_RIGHT) {
            if (dynamic_cast<CGoomba*>(e->obj)) {
				e->obj->SetState(GOOMBA_STATE_DIE);
            }

            if (dynamic_cast<CBrick*>(e->obj)) {
                if (e->nx > 0) {
                    SetState(TURTLE_STATE_SPIN_RIGHT);
                } else if (e->nx < 0) {
                    SetState(TURTLE_STATE_SPIN_LEFT);
                }
            }
        } else if (state == TURTLE_STATE_WALKING_LEFT || state == TURTLE_STATE_WALKING_RIGHT) {
            if (dynamic_cast<CBrick*>(e->obj)) {
                if (e->nx > 0) {
                    SetState(TURTLE_STATE_WALKING_RIGHT);
                } else if (e->nx < 0) {
                    SetState(TURTLE_STATE_WALKING_LEFT);
                }
            }
        }
    }
}


void CTurtle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if ((state == TURTLE_STATE_SPIN_LEFT || state == TURTLE_STATE_SPIN_RIGHT) && (GetTickCount64() - die_start > TURTLE_DIE_TIMEOUT*4))
	{
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CTurtle::Render()
{
	int aniId = 0;
	switch (GetState())
	{
		case TURTLE_STATE_DIE:
			aniId = ID_ANI_TURTLE_DIE;
			break;
		case TURTLE_STATE_SPIN_LEFT:
			aniId = ID_ANI_TURTLE_SPIN;
			break;
		case TURTLE_STATE_SPIN_RIGHT:
			aniId = ID_ANI_TURTLE_SPIN;
			break;
		case TURTLE_STATE_WALKING_RIGHT:
			aniId = ID_ANI_TURTLE_WALKING_RIGHT;
			break;
		case TURTLE_STATE_WALKING_LEFT:
			aniId = ID_ANI_TURTLE_WALKING_LEFT;
			break;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CTurtle::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case TURTLE_STATE_DIE:
		die_start = GetTickCount64();
		vx = 0;
		y += (TURTLE_BBOX_HEIGHT - TURTLE_BBOX_HEIGHT_DIE) / 2;
		vy = 0;
		ay = 0;
		break;
	case TURTLE_STATE_WALKING_LEFT:
		vx = -TURTLE_WALKING_SPEED;
		break;
	case TURTLE_STATE_WALKING_RIGHT:
		vx = TURTLE_WALKING_SPEED;
		break;
	case TURTLE_STATE_SPIN_RIGHT:
		die_start = GetTickCount64();
		vx = TURTLE_WALKING_SPEED*4;
		break;
	case TURTLE_STATE_SPIN_LEFT:
		die_start = GetTickCount64();
		vx = -TURTLE_WALKING_SPEED*4;
		break;
	}
}

#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"
#include "Box.h"
#include "Bullet.h"

#include "Goomba.h"
#include "Coin.h"
#include "Mushroom.h"
#include "Portal.h"
#include "PlayScene.h"
#include "Retangle.h"
#include "Collision.h"
#include "Flower.h"
#include "Turtle.h"
#include "BlackBox.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (abs(vx) > abs(maxVx)) vx = maxVx;

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	isOnPlatform = false;

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) {
			isOnPlatform = true;
			ay = MARIO_GRAVITY;
		};
	}
	else 
	if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
	}
	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CBox*>(e->obj))
		OnCollisionWithBox(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CMushroom*>(e->obj))
		OnCollisionWithMushroom(e);
	else if (dynamic_cast<CBullet*>(e->obj))
		OnCollisionWithBullet(e);
	else if (dynamic_cast<CFlower*>(e->obj))
		OnCollisionWithFlower(e);
	else if (dynamic_cast<CTurtle*>(e->obj))
		OnCollisionWithTurtle(e);
	else if (dynamic_cast<CBlackBox*>(e->obj))
		OnCollisionWithBlackBox(e);

}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			if (goomba->GetState() == GOOMBA_STATE_FLY || goomba->GetState() == GOOMBA_STATE_FLY_WALK) {
				goomba->SetState(GOOMBA_STATE_WALKING);
			}
			else {
				goomba->SetState(GOOMBA_STATE_DIE);
			}
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}


void CMario::OnCollisionWithTurtle(LPCOLLISIONEVENT e)
{
	CTurtle* turtle = dynamic_cast<CTurtle*>(e->obj);

	if (e->ny < 0)
	{
		if (turtle->GetState() == TURTLE_STATE_DIE) {
			float x_turtle;
			x_turtle = turtle->get_x();
			if (x < x_turtle) turtle->SetState(TURTLE_STATE_SPIN_RIGHT);
			if (x > x_turtle) turtle->SetState(TURTLE_STATE_SPIN_LEFT);

			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (turtle->GetState() == TURTLE_STATE_WALK_LEFT || turtle->GetState() == TURTLE_STATE_WALK_RIGHT)
		{
			turtle->SetState(TURTLE_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (turtle->GetState() == TURTLE_STATE_SPIN_LEFT || turtle->GetState() == TURTLE_STATE_SPIN_RIGHT) {
			turtle->tang_y();
			turtle->SetState(TURTLE_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}

	if (e->nx != 0 && turtle->GetState() != TURTLE_STATE_DIE)
	{
		if (untouchable == 0)
		{
			if (level > MARIO_LEVEL_SMALL)
			{
				level = MARIO_LEVEL_SMALL;
				StartUntouchable();
			}
			else
			{
				DebugOut(L">>> Mario DIE >>> \n");
				SetState(MARIO_STATE_DIE);
			}
		}
	}
	if (turtle->GetState() == TURTLE_STATE_DIE) {
		if ((state == MARIO_STATE_RUNNING_RIGHT || state == MARIO_STATE_RUNNING_LEFT) && hasTurtle == 0) {
			hasTurtle = 1;
			turtle->setHasMario();
		}
		else if (e->nx > 0) {
			turtle->SetState(TURTLE_STATE_SPIN_LEFT);
		}
		else if (e->nx < 0) {
			turtle->SetState(TURTLE_STATE_SPIN_RIGHT);
		}
	}

}

void CMario::OnCollisionWithFlower(LPCOLLISIONEVENT e)
{
		if (untouchable == 0)
		{
				if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					vy = -MARIO_JUMP_DEFLECT_SPEED;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
		}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	coin++;
}

void CMario::OnCollisionWithBullet(LPCOLLISIONEVENT e)
{
	e->obj->Delete();

	if (untouchable == 0)
	{
		
			if (level > MARIO_LEVEL_SMALL)
			{
				level = MARIO_LEVEL_SMALL;
				StartUntouchable();
			}
			else
			{
				DebugOut(L">>> Mario DIE >>> \n");
				SetState(MARIO_STATE_DIE);
			}
		
	}
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e)
{
	CMushroom* mushroom = dynamic_cast<CMushroom*>(e->obj);
	if (e != 0)
	{
		if (mushroom->getType() == 2) {
			if (level == MARIO_LEVEL_BIG) {
				SetLevel(MARIO_LEVEL_SUPER);
			}if (level == MARIO_LEVEL_SMALL) {
				SetLevel(MARIO_LEVEL_BIG);
			}
			
		}if (mushroom->getType() == 1) {
			SetLevel(MARIO_LEVEL_BIG);
		}		
		y -= 16.0f;
		e->obj->Delete();
	}
}

void CMario::OnCollisionWithBlackBox(LPCOLLISIONEVENT e)
{
	CBlackBox* bb = dynamic_cast<CBlackBox*>(e->obj);
	if (bb->GetState() != STATE_BLACK_BOX_ACTIVE) {
		bb->createInBox(bb->GetState());
		bb->SetState(STATE_BLACK_BOX_ACTIVE);
	}
}

void CMario::OnCollisionWithPlatform(LPCOLLISIONEVENT e)
{
	if (e->ny < 0) {
		vy = 0;
	this->isOnPlatform = true;
	}
}

void CMario::OnCollisionWithBox(LPCOLLISIONEVENT e)
{
	CBox* box = dynamic_cast<CBox*>(e->obj);
	if (e->ny > 0)
	{
		int state_box = box->GetState();
		if (state_box != STATE_BOX_ACTIVE) {
			if (box->getType() == 1) {
				if (box->getCoins() > 0) {
					coin++;
					box->createCoin();
					box->updateCoins();

					if (box->getCoins() == 0) box->SetState(STATE_BOX_ACTIVE);
				}
			}if (box->getType() == 2) {
				if (level == MARIO_LEVEL_BIG) {
					box->createMushroom(2);
				}if (level == MARIO_LEVEL_SMALL) {
					box->createMushroom(1);
				}if (level == MARIO_LEVEL_SUPER) {
					box->createMushroom(2);
				}
				box->SetState(STATE_BOX_ACTIVE);
			}
		}
	}
}
//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) {
					aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
					if (hasTurtle == 1) {
						aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT_RUA;
					}
				}
				else {
					aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
					if (hasTurtle == 1) {
						aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT_RUA;
					}
				}
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X) {
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
					if (hasTurtle == 1) {
						aniId = ID_ANI_MARIO_SMALL_RUN_RIGHT_RUA;
					}
				}
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X) {
					aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;

					if (hasTurtle == 1) {
						aniId = ID_ANI_MARIO_SMALL_RUN_LEFT_RUA;
					}

				}

				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}
//==============================================================================
int CMario::GetAniIdSuper()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT_DUOI;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT_DUOI;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT_DUOI;
			else
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT_DUOI;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) {
					aniId = ID_ANI_MARIO_IDLE_RIGHT_DUOI;
				}
				else {
					aniId = ID_ANI_MARIO_IDLE_LEFT_DUOI;
				}
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_BRACE_RIGHT_DUOI;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT_DUOI;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_RIGHT_DUOI;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT_DUOI;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_LEFT_DUOI;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_LEFT_DUOI;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT_DUOI;

	return aniId;
}

void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();
	else if (level == MARIO_LEVEL_SUPER)
		aniId = GetAniIdSuper();

	animations->Get(aniId)->Render(x, y);

	//RenderBoundingBox();
	
	DebugOutTitle(L"Coins: %d", coin);
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return; 
	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (level == MARIO_LEVEL_SUPER) ay = MARIO_GRAVITY / 1.5;
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;

	case MARIO_STATE_RELEASE_JUMP: // hạ cánh
		if (vy < 0) {
			switch (level)
			{
			case MARIO_LEVEL_BIG:
				vy += MARIO_JUMP_SPEED_Y / 2;
				break;
			case MARIO_LEVEL_SMALL:
				vy += MARIO_JUMP_SPEED_Y / 2;
				break;
			case MARIO_LEVEL_SUPER:
				ay = MARIO_GRAVITY / 3;
				vy += MARIO_JUMP_SPEED_Y / 2;
				break;
			default:
				break;
			}
		}
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y +=MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	}

	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (level==MARIO_LEVEL_BIG || level == MARIO_LEVEL_SUPER)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else 
		{
			left = x - MARIO_BIG_BBOX_WIDTH/2;
			top = y - MARIO_BIG_BBOX_HEIGHT/2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else
	{
		left = x - MARIO_SMALL_BBOX_WIDTH/2;
		top = y - MARIO_SMALL_BBOX_HEIGHT/2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = l;
}


#include "Flower.h"
#include "PlayScene.h"
#include "Bullet.h"
#include "Mario.h"

void CFlower::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + FLOWER_WIDTH;
	bottom = y + FLOWER_HEIGHT;
}

void CFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float mario_x, mario_y, x_dis, y_dis;
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	mario->GetPosition(mario_x, mario_y);

	x_dis = mario_x - x;
	
	if (x_dis > 0) {
		switch (state)
		{
			case FLOWER_STATE_FIRE_LEFT:
				state = FLOWER_STATE_FIRE_RIGHT;
				break;
			case FLOWER_STATE_NOL_LEFT:
				state = FLOWER_STATE_NOL_RIGHT;
				break;
		}
	}
	else {
		switch (state)
		{
		case FLOWER_STATE_FIRE_RIGHT:
			state = FLOWER_STATE_FIRE_LEFT;
			break;
		case FLOWER_STATE_NOL_RIGHT:
			state = FLOWER_STATE_NOL_LEFT;
			break;
		}

	}
	if (state == FLOWER_STATE_NOL_LEFT || state == FLOWER_STATE_NOL_RIGHT){
			if (y > y_temp - 23.0f)
			{
				y -= 0.5f;
				fire_time = GetTickCount64();
			}
			if ((y <= y_temp - 23.0f) &&(GetTickCount64() - fire_time > 1000)) {
				if (x_dis > 0) SetState(FLOWER_STATE_FIRE_RIGHT);
				if (x_dis <= 0) SetState(FLOWER_STATE_FIRE_LEFT);
			}
	}
	if (state == FLOWER_STATE_FIRE_LEFT || state == FLOWER_STATE_FIRE_RIGHT) {
		if (GetTickCount64() - fire_time > 500) {
			fireMario();

			if (x_dis > 0) SetState(FLOWER_STATE_NOL_LAN_RIGHT);
			if (x_dis <= 0) SetState(FLOWER_STATE_NOL_LAN_LEFT);
			
		}			
	}
	if (state == FLOWER_STATE_NOL_LAN_LEFT || state == FLOWER_STATE_NOL_LAN_RIGHT) {
		if (GetTickCount64() - fire_time > 1000) {
			if (y < y_temp)
			{
				y += 0.5f;
			}
			if (y == y_temp) {
				if (GetTickCount64() - fire_time > 4000) {
					float mario_x, mario_y, x_dis, y_dis;
					CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
					mario->GetPosition(mario_x, mario_y);

					x_dis = mario_x - x;
					y_dis = mario_y - y;

					float angle = atan2(y_dis, x_dis);
					if (abs(x_dis) < 200.0f) {
						if(x_dis <= 0) SetState(FLOWER_STATE_NOL_LEFT);
						if (x_dis > 0) SetState(FLOWER_STATE_NOL_RIGHT);
					}
				}
			}
		}
	
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CFlower::fireMario() {
	float mario_x, mario_y, x_dis, y_dis, x_bul, y_bul;
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	mario->GetPosition(mario_x, mario_y);

	// Tính toán góc bắn
	x_dis = mario_x - x;
	y_dis = mario_y - y;

	if (x_dis <= 0) x_bul = x - 6.0f;
	if (x_dis > 0) x_bul = x + 6.0f;

	float angle = atan2(y_dis, x_dis);

	// Tạo viên đạn và bắn
	if (abs(x_dis) < 400.0f) {
		LPSCENE a = CGame::GetInstance()->GetCurrentScene();
		CPlayScene* playScene = dynamic_cast<CPlayScene*>(a);
		CBullet* c = new CBullet(x_bul, y, angle);
		playScene->addObj(c);
	}
}


void CFlower::Render()
{
	int aniId = ID_ANI_FLOWER_LEFT;
	if (state == FLOWER_STATE_FIRE_LEFT)
	{
		aniId = ID_ANI_FLOWER_FIRE_LEFT;
	}
	if (state == FLOWER_STATE_FIRE_RIGHT)
	{
		aniId = ID_ANI_FLOWER_FIRE_RIGHT;
	}
	if (state == FLOWER_STATE_NOL_LEFT)
	{
		aniId = ID_ANI_FLOWER_LEFT;
	}
	if (state == FLOWER_STATE_NOL_RIGHT)
	{
		aniId = ID_ANI_FLOWER_RIGHT;
	}
	if (state == FLOWER_STATE_NOL_LAN_RIGHT)
	{
		aniId = ID_ANI_FLOWER_RIGHT;
	}
	if (state == FLOWER_STATE_NOL_LAN_LEFT)
	{
		aniId = ID_ANI_FLOWER_LEFT;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CFlower::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FLOWER_STATE_NOL_LAN_LEFT:
		fire_time = GetTickCount64();
		break;
	case FLOWER_STATE_NOL_LAN_RIGHT:
		fire_time = GetTickCount64();
		break;
	case FLOWER_STATE_FIRE_LEFT:
		fire_time = GetTickCount64();
		break;
	case FLOWER_STATE_FIRE_RIGHT:
		fire_time = GetTickCount64();
		break;
	case FLOWER_STATE_NOL_LEFT:
		fire_time = GetTickCount64();
		break;
	case FLOWER_STATE_NOL_RIGHT:
		fire_time = GetTickCount64();
		break;
	}
}

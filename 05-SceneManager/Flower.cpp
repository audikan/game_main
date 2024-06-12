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
	if (state == FLOWER_STATE_NOL){
			if (y > y_temp - 23.0f)
			{
				y -= 0.5f;
				fire_time = GetTickCount64();
			}
			if ((y <= y_temp - 23.0f) &&(GetTickCount64() - fire_time > 1000)) {
				SetState(FLOWER_STATE_FIRE);
			}
	}
	\
	if (state == FLOWER_STATE_FIRE) {
		if (GetTickCount64() - fire_time > 500) {
			fireMario();
			SetState(FLOWER_STATE_NOL_LAN);
		}			
	}
	if (state == FLOWER_STATE_NOL_LAN) {
		if (GetTickCount64() - fire_time > 1000) {
			if (y < y_temp)
			{
				y += 0.5f;
			}
			if (y == y_temp) {
				if (GetTickCount64() - fire_time == 4000) {
					SetState(FLOWER_STATE_NOL);
				}
			}
		}
	
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CFlower::fireMario() {
	float mario_x, mario_y, x_dis, y_dis;
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	mario->GetPosition(mario_x, mario_y);

	// Tính toán góc bắn
	x_dis = mario_x - x;
	y_dis = mario_y - y;

	float angle = atan2(y_dis, x_dis);

	// Tạo viên đạn và bắn
	LPSCENE a = CGame::GetInstance()->GetCurrentScene();
	CPlayScene* playScene = dynamic_cast<CPlayScene*>(a);
	CBullet* c = new CBullet(x - 3.0f, y, angle);
	playScene->addObj(c);
}


void CFlower::Render()
{
	int aniId = ID_ANI_FLOWER;
	if (state == FLOWER_STATE_FIRE)
	{
		aniId = ID_ANI_FLOWER_FIRE;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CFlower::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FLOWER_STATE_NOL_LAN:
		fire_time = GetTickCount64();
		break;
	case FLOWER_STATE_FIRE:
		fire_time = GetTickCount64();
		break;
	case FLOWER_STATE_NOL:
		fire_time = GetTickCount64();
		break;
	}
}

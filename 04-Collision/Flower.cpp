#include "Flower.h"
#include "Game.h"


void CFlower::Render()
{
	int ani = ID_ANI_FLOWER_NOL;
	if (this->GetState() == FLOWER_STATE_NOL) {
		ani = ID_ANI_FLOWER_NOL;
	}
	else {
		ani = ID_ANI_FLOWER_ATTACK;
	}
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ani)->Render(x, y);

}

void CFlower::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - FLOWER_BBOX_WIDTH / 2;
	t = y - FLOWER_BBOX_HEIGHT / 2;
	r = l + FLOWER_BBOX_WIDTH;
	b = t + FLOWER_BBOX_HEIGHT;
}
float CFlower::IsMarioNearby() {  //khoảng cách mario đến hoa
	float a, b, mario_y, mario_x;
	CGame::GetInstance()->GetCamPos(a,b);
	mario_x = a + 420;
	mario_y = b + 270;
	return mario_x;
}
void CFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float kc = IsMarioNearby();
	if (x - kc < 420) {
		if(y_temp - y < FLOWER_BBOX_HEIGHT)
			SetState(FLOWER_STATE_ATTACK);
	}
	else{
			SetState(FLOWER_STATE_NOL);
		}
	CGameObject::Update(dt);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CFlower::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FLOWER_STATE_ATTACK:
		y -= 0.01f;
		break;
	case FLOWER_STATE_NOL:
		y += 0.01f;;
		break;
	}
}
#include "BlackBox.h"
#include "PlayScene.h"

void CBlackBox::Render()
{
	int aniId = ID_ANI_BLACK_BOX_MUSHROOM;
	switch (state)
	{
	case STATE_BLACK_BOX_MUSHROOM:
		aniId = ID_ANI_BLACK_BOX_MUSHROOM;
		break;
	case STATE_BLACK_BOX_STAR:
		aniId = ID_ANI_BLACK_BOX_STAR;
		break;
	case STATE_BLACK_BOX_FLOWER:
		aniId = ID_ANI_BLACK_BOX_FLOWER;
		break;
	case STATE_BLACK_BOX_ACTIVE:
		aniId = ID_ANI_BLACK_BOX_ACTIVE;
		break;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CBlackBox::SetState(int state) {
	CGameObject::SetState(state);
	time_trans = GetTickCount64();

}
void CBlackBox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	if (GetTickCount64() - time_trans > 100) {
		switch (state)
		{
		case STATE_BLACK_BOX_MUSHROOM:
			SetState(STATE_BLACK_BOX_STAR);
			break;
		case STATE_BLACK_BOX_STAR:
			SetState(STATE_BLACK_BOX_FLOWER);
			break;
		case STATE_BLACK_BOX_FLOWER:
			SetState(STATE_BLACK_BOX_MUSHROOM);
			break;
		}
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CBlackBox::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BLACK_BOX_WIDTH / 2;
	t = y - BLACK_BOX_WIDTH / 2;
	r = l + BLACK_BOX_WIDTH;
	b = t + BLACK_BOX_WIDTH;
}
void CBlackBox::OnCollisionWith(LPCOLLISIONEVENT e) {
}
void CBlackBox::createInBox(int state)
{
	LPSCENE a = CGame::GetInstance()->GetCurrentScene();
	CPlayScene* playScene = dynamic_cast<CPlayScene*>(a);
	inCBlackBox* c = nullptr;
	if (state == STATE_BLACK_BOX_MUSHROOM) {
		 c = new inCBlackBox(3100, 160);
		 c->setType(1);
	}
	if (state == STATE_BLACK_BOX_FLOWER) {
		 c = new inCBlackBox(3100, 160);
		 c->setType(2);
	}
	if (state == STATE_BLACK_BOX_STAR) {
		 c = new inCBlackBox(3100, 160);
		 c->setType(3);
	}
	playScene->addObj(c);

}
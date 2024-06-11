#include "Box.h"
#include "Coin.h"
#include "Mushroom.h"
#include "PlayScene.h"

void CBox::Render()
{
	int aniId = ID_ANI_BOX_ACTIVE;
	if (state != STATE_BOX_ACTIVE)
	{
		aniId = ID_ANI_BOX;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CBox::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BOX_BBOX_WIDTH / 2;
	t = y - BOX_BBOX_HEIGHT / 2;
	r = l + BOX_BBOX_WIDTH;
	b = t + BOX_BBOX_HEIGHT;
}
void CBox::createCoin() {
	LPSCENE a = CGame::GetInstance()->GetCurrentScene();
	CPlayScene* playScene = dynamic_cast<CPlayScene*>(a);

	CCoin* c = new CCoin(x, y, 1);
	playScene->addObj(c);
}
void CBox::createMushroom() {
	LPSCENE a = CGame::GetInstance()->GetCurrentScene();
	CPlayScene* playScene = dynamic_cast<CPlayScene*>(a);

	CMushroom* c = new CMushroom(x, y);
	c->setInBox();
	playScene->addObj(c);
}

void CBox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (y_temp > y) {
		y += 1.0f;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
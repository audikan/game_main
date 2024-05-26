#include "Box.h"
#include "Mario.h"
#include "Coin.h"

void CBox::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BOX_BBOX_WIDTH / 2;
	t = y - BOX_BBOX_HEIGHT / 2;
	r = l + BOX_BBOX_WIDTH;
	b = t + BOX_BBOX_HEIGHT;
}
//------------------------
void CBox::OnCollisionWith(LPCOLLISIONEVENT e)
{
	CMario* box = dynamic_cast<CMario*>(e->obj);
	if (e->ny > 0) { // Mario đụng vào từ bên dưới
		if (state == BOX_STATE_UNACTIVE) {
			CreateCoin(); // Tạo đồng xu khi hộp được kích hoạt
		}
	}
}

void CBox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (y_temp > y) {
		y += 1.0f;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CBox::Render()
{
	int aniId = ID_ANI_BOX;
	if (state == BOX_STATE_ACTIVE)
	{
		aniId = ID_ANI_BOX_ACTIVE;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CBox::CreateCoin() {
	//LPGAMEOBJECT coin = new CCoin(x, y - BOX_BBOX_HEIGHT, true); // Tạo đồng xu tại vị trí của hộp
	//CGame::GetInstance()->AddObject(coin); // Thêm đồng xu vào game
}
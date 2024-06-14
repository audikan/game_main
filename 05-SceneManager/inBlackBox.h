#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
class inCBlackBox : public CGameObject {
public:
	int type;
	inCBlackBox(float x, float y) : CGameObject(x, y) {}
	void setType(int type) {
		this->type = type;
	}
	void Render() {
		CSprites* s = CSprites::GetInstance();
		int sprite;
		switch (type)
		{
		case 1:
			sprite = 73021;
			break;
		case 2:
			sprite = 73022;
			break;
		case 3:
			sprite = 73023;
			break;
		}
		s->Get(sprite)->Draw(x, y);
	};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
		y -= 2.0;
	};
	int IsBlocking() { return 0; }
	void GetBoundingBox(float& l, float& t, float& r, float& b) {};
};
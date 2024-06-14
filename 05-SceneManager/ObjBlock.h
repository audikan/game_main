#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BRICK 10000
#define OBJBLOCK_WIDTH 16
#define OBJBLOCK_BBOX_WIDTH 16
#define OBJBLOCK_BBOX_HEIGHT 16

class CObjBlock : public CGameObject {
protected:
	int sprite, width;
public:
	CObjBlock(float x, float y, int spt) : CGameObject(x, y) {
		width = 16;
		this->sprite = spt;
	}
	CObjBlock(float x, float y, int spt, int width) : CGameObject(x, y) {
		this->sprite = spt;
		this->width = width;
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};
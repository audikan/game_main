#include "ObjBlock.h"

void CObjBlock::Render()
{
	CSprites* s = CSprites::GetInstance();
	s->Get(sprite)->Draw(x, y);
}

void CObjBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BRICK_BBOX_WIDTH / 2;
	t = y - BRICK_BBOX_HEIGHT / 2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}
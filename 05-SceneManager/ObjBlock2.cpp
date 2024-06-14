#include "ObjBlock.h"

void CObjBlock::Render()
{
	CSprites* s = CSprites::GetInstance();
	s->Get(sprite)->Draw(x, y);
}

void CObjBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - width / 2;
	t = y - width / 2;
	r = l + width;
	b = t + width;
}
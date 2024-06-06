#pragma once
#include "GameObject.h"
#include "Sprite.h"
#include "Sprites.h"
#include "Textures.h"
class Background : public CGameObject {
protected:
	int id;
public:
	Background(int id, float x, float y) : CGameObject(x, y), id(id) {}
	void Render() {
		CSprites* s = CSprites::GetInstance();
		s->Get(id)->Draw(x, y);
	};
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b) {};
	void RenderBoundingBox() {};
};
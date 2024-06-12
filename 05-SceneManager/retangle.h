#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

class CRetangle : public CGameObject {
protected:
	int length;				// Unit: cell 
	float cellWidth;
	float cellHeight;
	int spriteId1, spriteId2, spriteId3, spriteId4, spriteId5, spriteId6, spriteId7, spriteId8, spriteId9;
	int height;

public:
	CRetangle(float x, float y,
		float cell_width, float cell_height, int length, int height,
		int sprite_id_1, int sprite_id_2, int sprite_id_3,
		int sprite_id_4, int sprite_id_5, int sprite_id_6,
		int sprite_id_7, int sprite_id_8, int sprite_id_9) : CGameObject(x, y) {
		this->length = length;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->height = height;

		this->spriteId1 = sprite_id_1;
		this->spriteId2 = sprite_id_2;
		this->spriteId3 = sprite_id_3;
		this->spriteId4 = sprite_id_4;
		this->spriteId5 = sprite_id_5;
		this->spriteId6 = sprite_id_6;
		this->spriteId7 = sprite_id_7;
		this->spriteId8 = sprite_id_8;
		this->spriteId9 = sprite_id_9;
	}
	void Render();
	void Update(DWORD dt) {}
	int IsDirectionColliable(float nx, float ny)
	{
		if (nx == 0 && ny == -1) return 1;
		else return 0;
	}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};
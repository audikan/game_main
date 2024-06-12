#include "retangle.h"
#include "Sprite.h"
#include "Sprites.h"

void CRetangle::Render()
{
    if (this->length <= 0) return;
    float xx = x;
    float yy = y;
    CSprites* s = CSprites::GetInstance();

    // Render cột đầu tiên
    s->Get(this->spriteId1)->Draw(xx, y);
    yy += this->cellWidth;
    if (this->height == 3) {
        s->Get(this->spriteId4)->Draw(xx, yy);
        yy += this->cellWidth;
    }
    for (int i = 1; i < this->height - 2; i++)
    {
        s->Get(this->spriteId4)->Draw(xx, yy);
        yy += this->cellWidth;
    }
    s->Get(this->spriteId7)->Draw(xx, yy);

    // Render các cột giữa
    xx += this->cellWidth;
    for (int i = 1; i < this->length - 1; i++)
    {
        yy = y;
        s->Get(this->spriteId2)->Draw(xx, yy);
        yy += this->cellWidth;
        if (this->height == 3) {
            s->Get(this->spriteId5)->Draw(xx, yy);
            yy += this->cellWidth;
        }
        for (int j = 1; j < this->height - 2; j++)
        {
            s->Get(this->spriteId5)->Draw(xx, yy);
            yy += this->cellWidth;
        }
        s->Get(this->spriteId8)->Draw(xx, yy);
        xx += this->cellWidth;
    }

    // Render cột cuối
    yy = y;
    s->Get(this->spriteId3)->Draw(xx, yy);
    yy += this->cellWidth;
    if (this->height == 3) {
        s->Get(this->spriteId6)->Draw(xx, yy);
        yy += this->cellWidth;
    }
    for (int i = 1; i < this->height - 2; i++)
    {
        s->Get(this->spriteId6)->Draw(xx, yy);
        yy += this->cellWidth;
    }
    s->Get(this->spriteId9)->Draw(xx, yy);

    // RenderBoundingBox();
}

void CRetangle::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    float cellWidth_div_2 = this->cellWidth / 2;
    l = x - cellWidth_div_2;
    t = y - this->cellHeight / 2;
    r = l + this->cellWidth * this->length;
    b = t + this->cellHeight;
}
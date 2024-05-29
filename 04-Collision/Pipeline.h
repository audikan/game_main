#pragma once
#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_PIPELINE 15000
#define PIPELINE_WIDTH 32
#define PIPELINE_BPIPELINE_WIDTH 32
#define PIPELINE_BPIPELINE_HEIGHT 32

class CPipeline : public CGameObject {
public:
	CPipeline(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};
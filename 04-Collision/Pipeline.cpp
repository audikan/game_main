#include "Pipeline.h"

void CPipeline::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_PIPELINE)->Render(x, y);
	RenderBoundingBox();
}

void CPipeline::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - PIPELINE_BPIPELINE_WIDTH / 2;
	t = y - PIPELINE_BPIPELINE_WIDTH / 2;
	r = l + PIPELINE_BPIPELINE_WIDTH;
	b = t + PIPELINE_BPIPELINE_WIDTH;
}
#ifndef FXPIPELINE_H
#define FXPIPELINE_H
#pragma once
#include "super_header.h"
#include "vertex_structs.h"

class FxPipeline
{
public:
	bool m_use = false;

	void CombineColorConst();
	void SetRenderPassForOpaqueColored();
	void SetRenderPassForOpaqueVertexColor();
	void SetRenderPassForOpaqueTextured();
	void SetRenderPassForOpaqueAlphaTest();
	void SetRenderPassForTranparentTextured();


protected:
};

#endif
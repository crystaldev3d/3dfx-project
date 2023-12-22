#include "pipeline.h"

void FxPipeline::CombineColorConst()
{
	/* set color to white */
	grColorCombine(GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE,
		GR_COMBINE_LOCAL_CONSTANT, GR_COMBINE_OTHER_NONE, FXFALSE);
	grConstantColorValue(~0); // white

}

void FxPipeline::SetRenderPassForOpaqueColored()
{
}

void FxPipeline::SetRenderPassForOpaqueVertexColor()
{
}

void FxPipeline::SetRenderPassForOpaqueTextured()
{
}

void FxPipeline::SetRenderPassForOpaqueAlphaTest()
{
}

void FxPipeline::SetRenderPassForTranparentTextured()
{
    grColorCombine(GR_COMBINE_FUNCTION_SCALE_OTHER,
        GR_COMBINE_FACTOR_LOCAL,
        GR_COMBINE_LOCAL_ITERATED,
        GR_COMBINE_OTHER_TEXTURE,
        FXFALSE);
    grAlphaCombine(GR_COMBINE_FUNCTION_SCALE_OTHER,
        GR_COMBINE_FACTOR_ONE,
        GR_COMBINE_LOCAL_NONE,
        GR_COMBINE_OTHER_TEXTURE,
        FXFALSE);
    grTexCombine(GR_TMU0,
        GR_COMBINE_FUNCTION_LOCAL,
        GR_COMBINE_FACTOR_NONE,
        GR_COMBINE_FUNCTION_LOCAL,
        GR_COMBINE_FACTOR_NONE,
        FXFALSE,
        FXFALSE);
    grAlphaBlendFunction(GR_BLEND_SRC_ALPHA, GR_BLEND_ONE_MINUS_SRC_ALPHA,
        GR_BLEND_ONE, GR_BLEND_ZERO);

}

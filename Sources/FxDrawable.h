#ifndef FXDRAWABLE_H
#define FXDRAWABLE_H
#pragma once
#include "super_header.h"
#include "vertex_structs.h"

class FxDrawable
{
public:
    
    void ViewProjectTransform(mat4 vp_tm);
    virtual void Draw();

    CGLM_ALIGN_MAT mat4 m_object_transform = GLM_MAT4_IDENTITY_INIT;
protected:
    bool RecalculateIndexBuffer();

    std::vector<GrVertex> m_verts;                  // raw data from file
    std::vector<GrVertex> m_verts_transformed;      // clipped and transformed data
    std::vector<GrVertex*> m_verts_transformed_ptr; // index Buffer
    
    CGLM_ALIGN_MAT mat4 model_view_projection = GLM_MAT4_IDENTITY_INIT;
    int primitiveType = GR_TRIANGLES;
};

#endif
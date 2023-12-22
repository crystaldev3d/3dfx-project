#ifndef FXCUSTOMMESH_H
#define FXCUSTOMMESH_H
#pragma once

#include "super_header.h"
#include "vertex_structs.h"
#include "FxDrawable.h"

class FxCustomMesh : public FxDrawable
{
public:
    void Begin(int gr_primitive_type);
    void AddVertex(vec3 pos);
    void AddVertex(vec3 pos, GrColor vertex_color);
    void AddVertex(vec3 pos, GrColor vertex_color, vec2 tmu0st);
    void AddVertex(vec3 pos, GrColor vertex_color, vec2 tmu0st, vec2 tum1st);
};
#endif /*FXCUSTOMMESH_H*/
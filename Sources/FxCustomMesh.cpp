#include "FxCustomMesh.h"
//#include "matrix.h"
//#include <glideutl.h>
//#include <glidesys.h>

void FxCustomMesh::Begin(int gr_primitive_type)
{
    primitiveType = gr_primitive_type;
}

void FxCustomMesh::AddVertex(vec3 pos)
{
    GrColor default_white {255, 255, 255, 255 };
    AddVertex(pos, default_white, vec2{0,0}, vec2{0,0});
}

void FxCustomMesh::AddVertex(vec3 pos, GrColor vertex_color)
{
    AddVertex(pos, vertex_color, vec2{ 0,0 }, vec2{ 0,0 });
}

void FxCustomMesh::AddVertex(vec3 pos, GrColor vertex_color, vec2 tmu0st)
{
    AddVertex(pos, vertex_color, tmu0st, vec2{ 0,0 });
}

void FxCustomMesh::AddVertex(vec3 pos, GrColor vertex_color, vec2 tmu0st, vec2 tmu1st)
{
    GrVertex v;
    v.x = pos[0];
    v.y = pos[1];
    v.z = pos[2];
    //v.w = 1.0f / pos[2];
    v.w = 1.0f;
    memcpy(&v.color, &vertex_color, sizeof(GrColor));
    glm_vec2_copy(tmu0st, v.tmuvtx[0].st);
    glm_vec2_copy(tmu1st, v.tmuvtx[1].st);

    m_verts.push_back(v);

}

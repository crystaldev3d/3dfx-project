#include "FxStaticModel.h"
#include "super_header.h"
#include "vertex_structs.h"
#include "matrix.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include "vertex_structs.h"

void FxStaticModel::Load(char* filename)
{
    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename)) {
        throw std::runtime_error(warn + err);
    }

    

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            GrVertex v{};
            vec2 tmu0st;
            vec2 tmu1st;

            v.x = attrib.vertices[3 * index.vertex_index + 0];
            v.y = attrib.vertices[3 * index.vertex_index + 1];
            v.z = attrib.vertices[3 * index.vertex_index + 2];
            //v.w = 1.0f / v.z;
            v.w = 1.0f;

            // TODO: BB merge

            // TODO: need implement using vertex color, now use default white for every vertex
            GrColor default_color = { 255,255,255, 255 }; // RGBA

            //GrColor default_color = { 255,255,255, 128 }; // semi transparent white

            memcpy(&v.color, &default_color, sizeof(GrColor));
            
            tmu0st[0] = attrib.texcoords[2 * index.texcoord_index + 0];
            tmu0st[1] = attrib.texcoords[2 * index.texcoord_index + 1];
            
            // TODO: Use same uvs for TMU2 ?
            glm_vec2_copy(tmu0st, v.tmuvtx[0].st);
            glm_vec2_copy(tmu1st, v.tmuvtx[1].st);

            m_verts.push_back(v);
        }
    }

    m_verts_transformed_ptr.resize(m_verts.size());
}


#include "FxDrawable.h"
#include "super_header.h"
#include "vertex_structs.h"
#include "matrix.h"
#include "vertex_structs.h"
#include "clip.h"
#include "framestat.h"

// planes definition = vec4{vec3=normal, distance}
static vec4 planes[6] = { {1.0f, 0.0f, 0.0f, 1.0f}, // left plane
                    {-1.0f, 0.0f, 0.0f, 1.0f},      // right    
                    {0.0f, 1.0f, 0.0f, 1.0f},       // top plane
                    {0.0f, -1.0f, 0.0f, 1.0f},      // bottom plane
                    {0.0f, 0.0f, 1.0f, 1.0f},
                    {0.0f, 0.0f, -1.0f, 1.0f} };

void FxDrawable::ViewProjectTransform(mat4 vp_tm)
{
    glm_mat4_mul(vp_tm, m_object_transform, model_view_projection);
    m_verts_transformed.clear();
    m_verts_transformed_ptr.clear();

    GrVertex v = { 0 };
    for (uint32_t i = 0; i < m_verts.size(); i++)
    {
        mat4_transform_GrVertex(model_view_projection, m_verts[i], v);
        m_verts_transformed.push_back(v);
        m_verts_transformed_ptr.push_back(&m_verts_transformed.back());
    }
}

void FxDrawable::Draw()
{
    uint32_t vertsNumBeforeClip = m_verts_transformed.size();
    for (uint32_t planeIndex = 0; planeIndex < 6; planeIndex++)
        clip_triangle(m_verts_transformed, planes[planeIndex]);

    uint32_t vertsNumAfterClip = vertsNumBeforeClip - m_verts_transformed.size();
    if (vertsNumAfterClip)
        g_render_stat.tris_clipped += vertsNumAfterClip / 3;

    //clip_triangle(m_verts_transformed, g_plane);

#if 0 // draw per triangle
    int vertexes_count = m_verts_transformed.size();
    if (vertexes_count) {
        g_render_stat.tris_rendered += vertexes_count / 3;
        for (int vertex_index = 0; vertex_index < vertexes_count; vertex_index += 3) {
            {
                grDrawTriangle(&m_verts_transformed[vertex_index], &m_verts_transformed[vertex_index + 1], &m_verts_transformed[vertex_index + 2]);
            }
        }
    }
#else // draw index buffer
    if (RecalculateIndexBuffer())
    {
        grDrawVertexArray(primitiveType, m_verts_transformed_ptr.size(), &m_verts_transformed_ptr[0]);
        g_render_stat.tris_rendered += m_verts_transformed_ptr.size() / 3;
    }
#endif

}

bool FxDrawable::RecalculateIndexBuffer()
{
    m_verts_transformed_ptr.clear();
    for (uint32_t i = 0; i < m_verts_transformed.size(); i++)
    {
        m_verts_transformed_ptr.push_back(&m_verts_transformed[i]);
    }

    return m_verts_transformed_ptr.size() >= 3; // true if we have at least one triangle
}

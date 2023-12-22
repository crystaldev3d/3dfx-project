#include "clip.h"
#include "super_header.h"

#include <iterator>
#include <algorithm>
#include "vertex_structs.h"
#include "camera.h"



inline float dot(const GrVertex& v1, const GrVertex& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline float dot(const GrVertex& v1, vec4 v2)
{
    return v1.x * v2[0] + v1.y * v2[1] + v1.z * v2[2];
}

void clip_triangle(std::vector<GrVertex>& vertexList, vec4 plane)
{
    size_t edge0, edge1;
    //size_t tri;
    size_t vertexListSize = vertexList.size();
    for (size_t tri = 0; tri < vertexListSize; tri += 3) {
        std::vector<GrVertex> polygon;
        for (edge0 = tri + 2, edge1 = tri; edge1 < tri + 3; edge0 = edge1++) {
            GrVertex point_current = vertexList[edge0];
            GrVertex point_next = vertexList[edge1];
            /* point.w is positive*/
            /* v.n - d = 0 */
            
            float dot0 = dot(point_current, plane) + point_current.w * plane[3];
            float dot1 = dot(point_next, plane) + point_next.w * plane[3];

            bool inside0 = dot0 >= 0.0f;
            bool inside1 = dot1 >= 0.0f;

            /* If start is inside, output it */
            if (inside0)
                polygon.push_back(point_current);
            if (inside0 != inside1) {
                /* We're clipping an edge */
                float t = 0.0f;
                float diff = 0.0f;
                /* swap vertices */
                if (!inside0) {
                    std::swap(point_current, point_next);
                    std::swap(dot0, dot1);
                }
                diff = dot0 - dot1;
                if (std::abs(diff) > 1e-10)
                    t = dot0 / diff;
                //GrVertex clip = point_current + (point_next - point_current) * t;
                //clip.w = point_current.w + (point_next.w - point_current.w) * t;
                GrVertex clip;
                memcpy(&clip, &point_current, sizeof(GrVertex));
                vec3 res;
                glm_vec3_sub(point_next.pos, point_current.pos, res);
                res[0] *= t;
                res[1] *= t;
                res[2] *= t;
                glm_vec3_add(point_current.pos, res, clip.pos);
                clip.w = point_current.w + (point_next.w - point_current.w) * t;

                // Here we doing interpolation for each vertex data

                // 1. Interpolate Vertex's UVs (for TMU0)
                vec2 uvres;
                glm_vec2_sub(point_next.tmuvtx[GR_TMU0].st, point_current.tmuvtx[GR_TMU0].st, uvres);
                uvres[0] *= t;
                uvres[1] *= t;
                glm_vec3_add(point_current.tmuvtx[GR_TMU0].st, uvres, clip.tmuvtx[GR_TMU0].st);

                // TODO: Add interpolation for Vetex - TMU1

                // 2. Interpolate Vertex's Color
                //vec4 resColor;
                //vec4 fromColor;
                //vec4 toColor;

                //fromColor[0] = point_current.color.r;
                //fromColor[1] = point_current.color.g;
                //fromColor[2] = point_current.color.b;
                //fromColor[3] = point_current.color.a;

                //toColor[0] = point_next.color.r;
                //toColor[1] = point_next.color.g;
                //toColor[2] = point_next.color.b;
                //toColor[3] = point_next.color.a;

                //glm_vec4_lerp(fromColor, toColor, t, resColor);

                //clip.color.r = (FxU8)resColor[0];
                //clip.color.g = (FxU8)resColor[1];
                //clip.color.b = (FxU8)resColor[2];
                //clip.color.a = (FxU8)resColor[3];


                int r1 = point_current.color.r;
                int r2 = point_next.color.r;

                int g1 = point_current.color.g;
                int g2 = point_next.color.g;

                int b1 = point_current.color.b;
                int b2 = point_next.color.b;

                int a1 = point_current.color.a;
                int a2 = point_next.color.a;

                clip.color.r = (int)((r2 - r1) * t + r1);
                clip.color.g = (int)((g2 - g1) * t + g1);
                clip.color.b = (int)((b2 - b1) * t + b1);
                clip.color.a = (int)((a2 - a1) * t + a1);

                polygon.push_back(clip);
            }
        }
        /* Split the resulting polygon into triangles */
        size_t vcount = polygon.size();
        if (vcount < 3)
            continue;
        for (size_t p = 1; p < vcount - 1; ++p) {
            polygon.push_back(polygon[0]);
            polygon.push_back(polygon[p]);
            polygon.push_back(polygon[p + 1]);
        }
        std::copy(polygon.begin() + vcount, polygon.end(), std::back_inserter(vertexList));
    }
    vertexList.erase(vertexList.begin(), vertexList.begin() + vertexListSize);
}

inline bool PointInFrustum(const GrVertex* v, const vec4 p[6])
{
    for (int i=0; i<6; i++) 
    {
        if ((p[i][0] * v->x + p[i][1] * v->y + p[i][2] * v->z) + p[i][3] <= 0.0f) 
        {
            return false;
        }
    }
    return true;
}

void clip_triangle(mat4 mvp, std::vector<GrVertex>& vertexList, std::vector<GrVertex>& clipped)
{
    vec4 planes[6] = {0};
    glm_frustum_planes(mvp, planes);
    size_t tris_count = vertexList.size() / 3;
    if (tris_count <= 0) return;
    clipped.resize(tris_count);

    for (size_t i=0; i < tris_count; i++)
    {
        int vertex_passed = 0;
        
        if (PointInFrustum(&vertexList[i*3+0], planes))
            vertex_passed++;

        if (PointInFrustum(&vertexList[i*3+1], planes))
            vertex_passed++;

        if (PointInFrustum(&vertexList[i*3+2], planes))
            vertex_passed++;

        if (vertex_passed==3)
        {
            clipped.push_back(vertexList[i * 3 + 0]);
            clipped.push_back(vertexList[i * 3 + 1]);
            clipped.push_back(vertexList[i * 3 + 2]);
        }
    }
}

bool clip_triangle(mat4 mvp, GrVertex& v0, GrVertex& v1, GrVertex& v2)
{
    vec4 planes[6] = { 0 };
    glm_frustum_planes(mvp, planes);

    int vertex_passed = 0;

    if (PointInFrustum(&v0, planes))
        vertex_passed++;
    else
        goto clip_triangle;

    if (PointInFrustum(&v1, planes))
        vertex_passed++;
    else
        goto clip_triangle;

    if (PointInFrustum(&v2, planes))
        vertex_passed++;
    else
        goto clip_triangle;


    if (vertex_passed == 3)
    {
        return true;
    }

 clip_triangle:
    return false;
}

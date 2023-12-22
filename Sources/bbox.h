#ifndef BBOX_H
#define BBOX_H

#pragma once
#include "super_header.h"
#include "vertex_structs.h"

static const float M_INFINITY = (float)INFINITY;

class BBox
{
public:
    BBox()
    {
        min[0] = -M_INFINITY; min[1] = -M_INFINITY; min[2] = -M_INFINITY;
        max[0] = M_INFINITY; max[1] = M_INFINITY; max[2] = M_INFINITY;
    };

    bool Defined() const
    {
        return min[0] != M_INFINITY;
    }

    void GetSize(vec3& ret) 
    { 
        ret[0] = max[0] - min[0];
        ret[1] = max[1] - min[1];
        ret[2] = max[2] - min[2];
    }

    void GetCenter(vec3& ret)
    {
        ret[0] = (max[0] + min[0]) * 0.5f;
        ret[1] = (max[1] + min[1]) * 0.5f;
        ret[2] = (max[2] + min[2]) * 0.5f;
    };

    void Merge(const vec3& point)
    {
        if (point[0] < min[0])
            min[0] = point[0];
        if (point[1] < min[1])
            min[1] = point[1];
        if (point[2] < min[2])
            min[2] = point[2];
        if (point[0] > max[0])
            max[0] = point[0];
        if (point[1] > max[1])
            max[1] = point[1];
        if (point[2] > max[2])
            max[2] = point[2];
    };

    bool IsInside(const vec3& point) const
    {
        if (point[0] < min[0] || point[0] > max[0] || point[1] < min[1] || point[1] > max[1] ||
            point[2] < min[2] || point[2] > max[2])
            return false;
        else
            return true;
    }

    BBox Transformed(const mat4& tm)
    {
        //vec3 newCenter;
        //GetCenter(newCenter);
        //newCenter = glm_mat4_mulv
        //vec3 size;
        //vec3 oldEdge;
        //GetSize(size);
        //oldEdge[0] = size[0] * 0.5f;
        //oldEdge[1] = size[1] * 0.5f;
        //oldEdge[2] = size[2] * 0.5f;
        //vec3 newEdge;

        //newEdge[0] = fabs(tm[0][0]) * oldEdge[0] + fabs(tm[0][1]) * oldEdge[1] + fabs(tm[0][2]) * oldEdge[2];
        //newEdge[1] = fabs(tm[1][0]) * oldEdge[0] + fabs(tm[1][1]) * oldEdge[1] + fabs(tm[1][2]) * oldEdge[2];
        //newEdge[2] = fabs(tm[2][0]) * oldEdge[0] + fabs(tm[2][1]) * oldEdge[1] + fabs(tm[2][2]) * oldEdge[2];

        //BBox ret;
        //ret.min[0] = 

        //vec3 oldEdge = GetSize()

    }


	vec3 min;
	vec3 max;
};


void mat4_transform_vec3(const mat4& m, const vec3& v, vec3 out); // depricated, no transforms for q
void mat4_transform_GrVertex(const mat4& m, const GrVertex& v, GrVertex& out);

#endif /*MATRIX_H*/
#pragma once
#include "super_header.h"

//typedef struct
//{
//    union
//    {
//        vec3 pos;
//        struct { FxFloat x, y, z; };
//    };
//    FxFloat q;
//    FxU32 pColor; //depends on GR_PARAM_PARGB
//} GrVertex_t;

typedef struct {
    union
    {
        vec2 st;
        struct { float  s; float  t; };
    };
}  GrTmuVertex;

typedef struct
{
    union 
    { 
        FxU8 v[4];
        FxU32 value;
        struct { FxU8 a, r, g, b; };
    };
} GrColor;

typedef struct
{
    union {
        vec4 pos;
        struct { FxFloat x, y, z, w; };
    };
    //FxU32 rgba;
    GrColor color;
    GrTmuVertex  tmuvtx[GLIDE_NUM_TMU];
} GrVertex;

void SetupVertexLayout(void);
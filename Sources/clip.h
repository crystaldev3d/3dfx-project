#include <glide.h>
#include <cglm/cglm.h>

#include "vertex_structs.h"

void clip_triangle(mat4 mvp, std::vector<GrVertex>& vertexList, std::vector<GrVertex>& clipped);
bool clip_triangle(mat4 mvp, GrVertex& v0, GrVertex& v1, GrVertex& v2);
void clip_triangle(std::vector<GrVertex>& vertexList, vec4 plane);
//void near_clip(GrVertex a, GrVertex b, GrVertex c, GrVertex d);


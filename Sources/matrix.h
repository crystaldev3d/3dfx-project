
#ifndef MATRIX_H
#define MATRIX_H

#pragma once
#include "super_header.h"
#include "vertex_structs.h"

// https://github.com/alextrevisan/3dfx-glide-game-engine/blob/master/src/engine/Matrix4.cpp

void mat4_transform_vec3(const mat4& m, const vec3& v, vec3 out); // depricated, no transforms for q
void mat4_transform_GrVertex(const mat4& m, const GrVertex& v, GrVertex& out);

#endif /*MATRIX_H*/
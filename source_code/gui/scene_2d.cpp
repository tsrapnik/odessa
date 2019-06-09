#pragma once

#include "scene_2d.h"

void scene_2d::append_triangle(vector_2_f32 vertex_0, vector_2_f32 vertex_1, vector_2_f32 vertex_2, color a_color)
{
    u32 index_0 = vertices.get_size();
    this->triangles.append_copy({index_0, index_0 + 1, index_0 + 2});
    this->vertices.append_copy({vertex_0, a_color});
    this->vertices.append_copy({vertex_1, a_color});
    this->vertices.append_copy({vertex_2, a_color});
}
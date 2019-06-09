#include "scene_2d.h"

#include "math.h"

void scene_2d::append_triangle(vector_2_f32 vertex_0, vector_2_f32 vertex_1, vector_2_f32 vertex_2, color a_color)
{
    u32 index_0 = vertices.get_size();
    this->triangles.append_copy({index_0, index_0 + 1, index_0 + 2});
    this->vertices.append_copy({vertex_0, a_color});
    this->vertices.append_copy({vertex_1, a_color});
    this->vertices.append_copy({vertex_2, a_color});
}

void scene_2d::append_line(vector_2_f32 vertex_0, vector_2_f32 vertex_1, color a_color, f32 thickness)
{
    u32 index_0 = vertices.get_size();
    this->triangles.append_copy({index_0, index_0 + 1, index_0 + 2});
    this->triangles.append_copy({index_0 + 2, index_0 + 3, index_0});

    vector_2_f32 line_vector = vector_2_f32::difference(vertex_1, vertex_0);
    f32 unity_factor = 1.0f / math::sqrt(line_vector.coordinate[0] * line_vector.coordinate[0] + line_vector.coordinate[1] * line_vector.coordinate[1]);
    vector_2_f32 half_thickness_vector = vector_2_f32::scalar_product(unity_factor * thickness / 2.0f,
                                                                      vector_2_f32(-line_vector.coordinate[1], line_vector.coordinate[0]));

    this->vertices.append_copy({vector_2_f32::sum(vertex_0, half_thickness_vector), a_color});
    this->vertices.append_copy({vector_2_f32::sum(vertex_1, half_thickness_vector), a_color});
    this->vertices.append_copy({vector_2_f32::difference(vertex_1, half_thickness_vector), a_color});
    this->vertices.append_copy({vector_2_f32::difference(vertex_0, half_thickness_vector), a_color});
}
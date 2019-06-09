#pragma once

#include "color.h"
#include "list.h"
#include "vector.h"

class scene_2d
{
    //todo: consider making members private and access with functions.
public:
    struct vertex
    {
        vector_2_f32 position;
        color a_color;
    };

    struct triangle
    {
        u32 index_0;
        u32 index_1;
        u32 index_2;
    };
    
    list<vertex> vertices;
    list<triangle> triangles;
    
	void append_triangle(vector_2_f32 vertex_0, vector_2_f32 vertex_1, vector_2_f32 vertex_3, color a_color);
    void append_line(vector_2_f32 vertex_0, vector_2_f32 vertex_1, color a_color, f32 thickness = 1.0f);
};
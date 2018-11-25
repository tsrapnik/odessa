#pragma once
#include "mailbox_property_tags.h"
#include "type_definitions.h"

#include "screen.h" //todo: remove.
#include "string.h"

class gpu
{
    public:
    //in screen coordinates.
    struct vertex
    {
        u16 x; //in 12.4 fixed point format.
        u16 y; //in 12.4 fixed point format.
        f32 z;
        f32 w;
        f32 r;
        f32 g;
        f32 b;
    } __attribute__((packed));

    struct triangle
    {
        u8 index_0;
        u8 index_1;
        u8 index_2;
    } __attribute__((packed));

    private:
    mailbox_property_tags& a_mailbox_property_tags;

    u32 vertex_buffer_handle;
    u32 shader_handle;
    u32 render_control;
    u32 tile_state_size;
    u32 tile_state_handle;
    u32 tile_state;
    u32 binning_data_handle;
    u32 binning_data;


    public:
    screen* a_screen; //todo: remove.
    gpu(u32 vertex_buffer_size, u32 triangle_buffer_size);
    ~gpu();
    void render(u16 width, u16 height, u32 frame_buffer, vertex* new_vertex_buffer, triangle* new_triangle_buffer, u32 vertex_buffer_size, u32 triangle_buffer_size);
};
#pragma once

#include "output.h"
#include "scene_2d.h"

class input
{
    private:
    graphic* the_graphic;
    output* source;

    public:
    input(rectangle footprint, color own_color);
    ~input();

    void connect_output(output* source);
    f32 get_frame();
    bool frame_ready();

    rectangle get_bounding_box();
    void draw(scene_2d& scene);
    void draw_connection(scene_2d& scene);
    bool is_selected(vector_2_f32 mouse_position);
    void move(vector_2_f32 displacement);
};
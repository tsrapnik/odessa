#pragma once

#include "button.h"

class output
{
    private:
    graphic* the_graphic;
    f32 frame;
    bool ready;

    public:
    output(rectangle bounding_box, color the_color);
    ~output();

    f32 get_frame();
    void update_frame(f32 frame);
    void invalidate_frame();
    bool frame_ready();

    rectangle get_bounding_box();
	void draw(scene_2d& scene);
    void draw_connection(scene_2d& scene, vector_2_f32 mouse_position);
    bool is_selected(vector_2_f32 mouse_position);
    void move(vector_2_f32 displacement);
};
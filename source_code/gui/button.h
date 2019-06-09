#pragma once

#include "graphic.h"
#include "rectangle.h"

class button
{
    private:
        graphic* the_graphic;
    public:
    button(rectangle bounding_box, color the_color);
    virtual ~button();

    rectangle get_bounding_box();
    void draw(scene_2d& scene);
    bool is_selected(vector_2_f32 mouse_position);
    void move(vector_2_f32 displacement);
};
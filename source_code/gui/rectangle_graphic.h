#pragma once

#include "color.h"
#include "rectangle.h"

class rectangle_graphic
{
    protected:
    rectangle footprint;
    color own_color;

    public:
    rectangle_graphic(rectangle footprint, color own_color);
    rectangle get_footprint();
    void set_footprint(rectangle new_footprint);
    color get_color();
    virtual ~rectangle_graphic() = 0;
    void draw();
    virtual vector_2_u32 move(vector_2_u32 displacement);
    rectangle_graphic* is_selected(vector_2_u32 mouse_pointer);
};
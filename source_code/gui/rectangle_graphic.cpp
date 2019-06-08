#include "rectangle_graphic.h"

rectangle_graphic::rectangle_graphic(rectangle footprint, color own_color)
{
    this->footprint = footprint;
    this->boundaries = nullptr;
    this->own_screen = nullptr;
    this->own_color = own_color;
}

void rectangle_graphic::set_boundaries(rectangle* boundaries)
{
    if(footprint.origin.coordinate[0] < 0)
        footprint.origin.coordinate[0] = 0;
    if(footprint.origin.coordinate[0] + footprint.size.coordinate[0] > boundaries->size.coordinate[0])
        footprint.size.coordinate[0] = boundaries->size.coordinate[0] - footprint.origin.coordinate[0];
    if(footprint.origin.coordinate[1] < 0)
        footprint.origin.coordinate[1] = 0;
    if(footprint.origin.coordinate[1] + footprint.size.coordinate[1] > boundaries->size.coordinate[1])
        footprint.size.coordinate[1] = boundaries->size.coordinate[1] - footprint.origin.coordinate[1];

    footprint.origin = vector_2_u32::sum(footprint.origin, boundaries->origin);
    this->boundaries = boundaries;
}

void rectangle_graphic::set_own_screen(screen* own_screen)
{
    this->own_screen = own_screen;
}

rectangle rectangle_graphic::get_footprint()
{
    return footprint;
}

void rectangle_graphic::set_footprint(rectangle new_footprint)
{
    footprint = new_footprint;
}

color rectangle_graphic::get_color()
{
    return own_color;
}

rectangle_graphic::~rectangle_graphic()
{
}

void rectangle_graphic::draw()
{
    for(u32 x = footprint.origin.coordinate[0]; x < footprint.origin.coordinate[0] + footprint.size.coordinate[0]; x++)
        for(u32 y = footprint.origin.coordinate[1]; y < footprint.origin.coordinate[1] + footprint.size.coordinate[1]; y++)
            own_screen->put_transparent_pixel(vector_2_u32(x, y), own_color);
}

vector_2_u32 rectangle_graphic::move(vector_2_u32 displacement)
{
    if(footprint.origin.coordinate[0] + displacement.coordinate[0] < boundaries->origin.coordinate[0])
        displacement.coordinate[0] = boundaries->origin.coordinate[0] - footprint.origin.coordinate[0];
    if(footprint.origin.coordinate[0] + footprint.size.coordinate[0] + displacement.coordinate[0] > boundaries->origin.coordinate[0] + boundaries->size.coordinate[0])
        displacement.coordinate[0] = boundaries->origin.coordinate[0] + boundaries->size.coordinate[0] - footprint.origin.coordinate[0] - footprint.size.coordinate[0];
    if(footprint.origin.coordinate[1] + displacement.coordinate[1] < boundaries->origin.coordinate[1])
        displacement.coordinate[1] = boundaries->origin.coordinate[1] - footprint.origin.coordinate[1];
    if(footprint.origin.coordinate[1] + footprint.size.coordinate[1] + displacement.coordinate[1] > boundaries->origin.coordinate[1] + boundaries->size.coordinate[1])
        displacement.coordinate[1] = boundaries->origin.coordinate[1] + boundaries->size.coordinate[1] - footprint.origin.coordinate[1] - footprint.size.coordinate[1];

    footprint.origin = vector_2_u32::sum(footprint.origin, displacement);

    return displacement;
}

rectangle_graphic* rectangle_graphic::is_selected(vector_2_u32 mouse_pointer)
{
    if(mouse_pointer.coordinate[0] >= footprint.origin.coordinate[0] &&
       mouse_pointer.coordinate[0] < footprint.origin.coordinate[0] + footprint.size.coordinate[0] &&
       mouse_pointer.coordinate[1] >= footprint.origin.coordinate[1] &&
       mouse_pointer.coordinate[1] < footprint.origin.coordinate[1] + footprint.size.coordinate[1])
        return this;
    return nullptr;
}

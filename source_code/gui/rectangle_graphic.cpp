#include "rectangle_graphic.h"

rectangle_graphic::rectangle_graphic(rectangle footprint, color own_color)
{
    this->footprint = footprint;
    this->own_color = own_color;
}

rectangle rectangle_graphic::get_footprint()
{
    return this->footprint;
}

void rectangle_graphic::set_footprint(rectangle new_footprint)
{
    this->footprint = new_footprint;
}

color rectangle_graphic::get_color()
{
    return this->own_color;
}

void rectangle_graphic::set_color(color new_color)
{
    this->own_color = new_color;
}

rectangle_graphic::~rectangle_graphic()
{
}

void rectangle_graphic::draw(scene_2d& scene)
{
    scene.append_triangle(footprint.origin,
                          vector_2_f32(footprint.origin.coordinate[0] + footprint.size.coordinate[0], footprint.origin.coordinate[1]),
                          vector_2_f32::sum(footprint.origin, footprint.size),
                          own_color);

    scene.append_triangle(vector_2_f32::sum(footprint.origin, footprint.size),
                          vector_2_f32(footprint.origin.coordinate[0], footprint.origin.coordinate[1] + footprint.size.coordinate[1]),
                          footprint.origin,
                          own_color);
}

bool rectangle_graphic::is_selected(vector_2_f32 position)
{
    return (position.coordinate[0] >= footprint.origin.coordinate[0] &&
            position.coordinate[0] < footprint.origin.coordinate[0] + footprint.size.coordinate[0] &&
            position.coordinate[1] >= footprint.origin.coordinate[1] &&
            position.coordinate[1] < footprint.origin.coordinate[1] + footprint.size.coordinate[1]);
}

void rectangle_graphic::move(vector_2_f32 movement)
{
    footprint.origin = vector_2_f32::sum(footprint.origin, movement);
}
#include "graphic_rectangle.h"

graphic_rectangle::graphic_rectangle(rectangle bounding_box, color the_color)
{
    this->bounding_box = bounding_box;
    this->the_color = the_color;
}

void graphic_rectangle::set_bounding_box(rectangle new_bounding_box)
{
    this->bounding_box = new_bounding_box;
}

color graphic_rectangle::get_color()
{
    return this->the_color;
}

void graphic_rectangle::set_color(color new_color)
{
    this->the_color = new_color;
}

graphic_rectangle::~graphic_rectangle()
{
}

void graphic_rectangle::draw(scene_2d& scene)
{
    scene.append_triangle(bounding_box.origin,
                          vector_2_f32(bounding_box.origin.coordinate[0] + bounding_box.size.coordinate[0], bounding_box.origin.coordinate[1]),
                          vector_2_f32::sum(bounding_box.origin, bounding_box.size),
                          the_color);

    scene.append_triangle(vector_2_f32::sum(bounding_box.origin, bounding_box.size),
                          vector_2_f32(bounding_box.origin.coordinate[0], bounding_box.origin.coordinate[1] + bounding_box.size.coordinate[1]),
                          bounding_box.origin,
                          the_color);
}

bool graphic_rectangle::is_selected(vector_2_f32 mouse_position)
{
    return (mouse_position.coordinate[0] >= bounding_box.origin.coordinate[0] &&
            mouse_position.coordinate[0] < bounding_box.origin.coordinate[0] + bounding_box.size.coordinate[0] &&
            mouse_position.coordinate[1] >= bounding_box.origin.coordinate[1] &&
            mouse_position.coordinate[1] < bounding_box.origin.coordinate[1] + bounding_box.size.coordinate[1]);
}

void graphic_rectangle::move(vector_2_f32 displacement)
{
    this->bounding_box.origin = vector_2_f32::sum(this->bounding_box.origin, displacement);
}

rectangle graphic_rectangle::get_bounding_box()
{
    return this->bounding_box;
}
#include "input.h"

#include "graphic_rectangle.h"

input::input(rectangle footprint, color own_color) :
    the_graphic(new graphic_rectangle(footprint, own_color)),
    source(nullptr)
{
}

input::~input()
{
}

void input::connect_output(output* source)
{
    this->source = source;
}

f32 input::get_frame()
{
    if(source)
        return source->get_frame();
    return 0.0f; //if there is no source connected return 0.0f.
}

bool input::frame_ready()
{
    if(source)
        return source->frame_ready();
    return true; //if there is no source connected the frame is always considered ready, the returned frame will always be 0.0f then.
}

rectangle input::get_bounding_box()
{
    return this->the_graphic->get_bounding_box();
}

void input::draw(scene_2d& scene)
{
    this->the_graphic->draw(scene);
}

void input::draw_connection(scene_2d& scene)
{
    if(source != nullptr)
    {
        scene.append_line(source->get_bounding_box().get_center(),
                          this->the_graphic->get_bounding_box().get_center(),
                          color(255, 255, 255, 255));
    }
}

bool input::is_selected(vector_2_f32 mouse_position)
{
	return the_graphic->is_selected(mouse_position);
}

void input::move(vector_2_f32 displacement)
{
	the_graphic->move(displacement);
}

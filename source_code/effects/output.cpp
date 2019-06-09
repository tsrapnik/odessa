#include "output.h"

#include "graphic_rectangle.h"

output::output(rectangle footprint, color own_color) :
    the_graphic(new graphic_rectangle(footprint, own_color)),
    frame(0.0f),
    ready(false)
{
}

output::~output()
{
}

f32 output::get_frame()
{
    return this->frame;
}

void output::update_frame(f32 frame)
{
    this->frame = frame;
    this->ready = true;
}

void output::invalidate_frame()
{
    this->ready = false;
}

bool output::frame_ready()
{
    return this->ready;
}

rectangle output::get_bounding_box()
{
    return this->the_graphic->get_bounding_box();
}

void output::draw(scene_2d& scene)
{
	the_graphic->draw(scene);
}

void output::draw_connection(scene_2d& scene, vector_2_f32 mouse_position)
{
    scene.append_line(this->the_graphic->get_bounding_box().get_center(),
                      mouse_position,
                      color(255, 255, 255, 255));
}

bool output::is_selected(vector_2_f32 mouse_position)
{
	return the_graphic->is_selected(mouse_position);
}

void output::move(vector_2_f32 displacement)
{
	the_graphic->move(displacement);
}

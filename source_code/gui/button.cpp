#include "button.h"

#include "graphic_rectangle.h"

button::button(rectangle bounding_box, color the_color) :
    the_graphic(new graphic_rectangle(bounding_box, the_color))
{
}

button::~button()
{
	delete the_graphic;
}

rectangle button::get_bounding_box()
{
    return this->the_graphic->get_bounding_box();
}

void button::draw(scene_2d& scene)
{
    this->the_graphic->draw(scene);
}

bool button::is_selected(vector_2_f32 mouse_position)
{
	return the_graphic->is_selected(mouse_position);
}

void button::move(vector_2_f32 displacement)
{
	the_graphic->move(displacement);
}

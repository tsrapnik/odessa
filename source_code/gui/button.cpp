#include "button.h"

#include "rectangle_graphic.h"

button::button(rectangle footprint, color own_color) :
    the_graphic(new rectangle_graphic(footprint, own_color))
{
}

button::~button()
{
	delete the_graphic;
}

#include "push_button.h"

push_button::push_button(rectangle bounding_box, color the_color) :
    button(bounding_box, the_color)
{
}

push_button::~push_button()
{
}

void push_button::set_pushed()
{
    pushed = true;
}

//check if a positive edge was triggered, reset after read.
bool push_button::check_pushed()
{
    bool temp = pushed;
    pushed = false;
    return temp;
}

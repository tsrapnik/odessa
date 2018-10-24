#include "push_button.h"

push_button::push_button(rectangle footprint, color own_color) :
    button(footprint, own_color)
{
}

push_button::~push_button()
{
}

void push_button::set_pushed()
{
}

//check if a positive edge was triggered, reset after read.
bool push_button::check_pushed()
{
    bool temp = pushed;
    pushed = false;
    return temp;
}

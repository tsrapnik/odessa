#include "push_button.h"

push_button::push_button( rectangle footprint, colour own_colour ):
	button( footprint, own_colour )
{
}

push_button::~push_button()
{
}

void push_button::set_pushed()
{
}

///<summary>
///check if a positive edge was triggered, reset after read.
///</summary>
bool push_button::check_pushed()
{
	bool temp = pushed;
	pushed = false;
	return temp;
}

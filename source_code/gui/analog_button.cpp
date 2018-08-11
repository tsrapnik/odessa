#include "analog_button.h"

analog_button::analog_button( rectangle footprint, colour own_colour ):
	button( footprint, own_colour )
{
	value = 0.0f;
}

analog_button::~analog_button()
{}

double analog_button::get_value()
{
	return value;
}

void analog_button::set_value( double value )
{
	this->value = value;
}

#include "analog_button.h"

analog_button::analog_button( rectangle footprint, color own_color ):
	button( footprint, own_color )
{
	value = 0.0f;
}

analog_button::~analog_button()
{}

f64 analog_button::get_value()
{
	return value;
}

void analog_button::set_value( f64 value )
{
	this->value = value;
}

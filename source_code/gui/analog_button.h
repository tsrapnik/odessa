#pragma once

#include "button.h"

class analog_button:
	public button
{
private:
	double value;
public:
	analog_button( rectangle footprint, colour own_colour );
	~analog_button();
	double get_value();
	void set_value( double value );
};
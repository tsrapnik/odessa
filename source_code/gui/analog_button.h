#pragma once

#include "button.h"

class analog_button:
	public button
{
private:
	f64 value;
public:
	analog_button( rectangle footprint, color own_color );
	~analog_button();
	f64 get_value();
	void set_value( f64 value );
};
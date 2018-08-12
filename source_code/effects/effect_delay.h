#pragma once

#include "effect.h"
#include "analog_button.h"
#include "buffer_delay.h"

class effect_delay: public effect
{
private:
	input* mono_input;
	output* mono_output;
	analog_button* feedback_button, * wet_button, * delay_time_button;

	buffer_delay delay;

	void process();
public:
	effect_delay( rectangle footprint, colour own_colour );
	~effect_delay();
};
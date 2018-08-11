#pragma once

#include "effect.h"

class effect_sink:
	public effect
{
private:
	///
	///points to the actual 32 bit value that is sent to the audio output, e.g. dac.
	///
	signed int* sink;

	input* mono_input;

	virtual void process();
public:
	effect_sink( signed int* sink, rectangle footprint, colour own_colour );
	~effect_sink();
};
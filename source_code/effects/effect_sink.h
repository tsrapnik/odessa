#pragma once

#include "effect.h"

class effect_sink:
	public effect
{
private:
	///
	///points to the actual 32 bit value that is sent to the audio output, e.g. dac.
	///
	i32* sink;

	input* mono_input;

	virtual void process();
public:
	effect_sink( i32* sink, rectangle footprint, color own_color );
	~effect_sink();
};
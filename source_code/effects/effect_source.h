#pragma once

#include "effect.h"

class effect_source:
	public effect
{
private:
	///
	///points to the actual 32 bit value that is received from the audio intput, e.g. adc.
	///
	i32* source;

	output* mono_output;

	virtual void process();
public:
	effect_source( i32* source, rectangle footprint, color own_color );
	~effect_source
();
};
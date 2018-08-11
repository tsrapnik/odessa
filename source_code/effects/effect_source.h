#pragma once

#include "effect.h"

class effect_source:
	public effect
{
private:
	///
	///points to the actual 32 bit value that is received from the audio intput, e.g. adc.
	///
	signed int* source;

	output* mono_output;

	virtual void process();
public:
	effect_source( signed int* source, rectangle footprint, colour own_colour );
	~effect_source
();
};
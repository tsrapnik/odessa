#pragma once

#include "effect.h"

class effectSource:
	public effect
{
private:
	///
	///Points to the actual 32 bit value that is received from the audio intput, e.g. adc.
	///
	signed int* source;

	output* monoOutput;

	virtual void process();
public:
	effectSource( signed int* source, rectangle footprint, colour ownColour );
	~effectSource
();
};
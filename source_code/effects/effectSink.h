#pragma once

#include "effect.h"

class effectSink:
	public effect
{
private:
	///
	///Points to the actual 32 bit value that is sent to the audio output, e.g. dac.
	///
	signed int* sink;

	input* monoInput;

	virtual void process();
public:
	effectSink( signed int* sink, rectangle footprint, colour ownColour );
	~effectSink();
};
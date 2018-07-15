#pragma once

#include "effect.h"
#include "analogButton.h"
#include "bufferDelay.h"

class effectTsrapnikDelay: public effect
{
private:
	input* monoInput;
	output* monoOutput;
	analogButton* feedbackButton, * wetButton, * delayTimeButton;

	bufferDelay delay;

	void process();
public:
	effectTsrapnikDelay( rectangle footprint, colour ownColour );
	~effectTsrapnikDelay();
};
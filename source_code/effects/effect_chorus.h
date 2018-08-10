#pragma once

#include "effect.h"
#include "analogButton.h"
#include "bufferDelay.h"

class effectTsrapnikChorus: public effect
{
private:
	input* monoInput;
	output* monoOutput;
	analogButton* feedbackButton,* wetButton;

	bufferDelay delay;

	void process();
public:
	effectTsrapnikChorus( rectangle footprint, colour ownColour );
	~effectTsrapnikChorus();
};
#pragma once

#include "output.h"

class input:
	public graphicObject
{
private:
	output* source;
public:
	input( rectangle footprint, colour ownColour );
	~input();
	void connectOutput( output* source );
	double getFrame();
	bool frameReady();
	void drawConnection();
};
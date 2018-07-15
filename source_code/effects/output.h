#pragma once

#include "button.h"

class output:
	public graphicObject
{
private:
	double frame;
	bool ready;
public:
	output( rectangle footprint, colour ownColour );
	~output();
	double getFrame();
	void updateFrame( double frame );
	void invalidateFrame();
	bool frameReady();
	void drawConnecting( vector2I mousePointer );
};
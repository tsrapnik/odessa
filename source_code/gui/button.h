#pragma once

#include "graphicObject.h"

class button:
	public graphicObject
{
public:
	button( rectangle footprint, colour ownColour );
	~button();
};
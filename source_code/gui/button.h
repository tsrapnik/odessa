#pragma once

#include "graphic_object.h"

class button:
	public graphic_object
{
public:
	button( rectangle footprint, colour own_colour );
	~button();
};
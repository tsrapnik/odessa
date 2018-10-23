#pragma once

#include "graphic_object.h"

class button:
	public graphic_object
{
public:
	button( rectangle footprint, color own_color );
	~button();
};
#pragma once

#include "rectangle.h"
#include "colour.h"
#include "screen.h"

class graphic_object
{
protected:
	rectangle footprint;
	rectangle* boundaries;
	screen* own_screen;
	colour own_colour;
public:
	//ownscreen and boundaries should be set before using any other members.
	graphic_object( rectangle footprint, colour own_colour );
	void set_boundaries( rectangle* boundaries );
	void set_own_screen( screen* own_screen );
	rectangle get_footprint();
	void set_footprint( rectangle new_footprint );
	colour get_colour();
	virtual ~graphic_object() = 0;
	void draw();
	virtual vector2_i move( vector2_i displacement );
	graphic_object* is_selected( vector2_i mouse_pointer );
};
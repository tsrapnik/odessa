#pragma once

#include "rectangle.h"
#include "color.h"
#include "screen.h"

class graphic_object
{
protected:
	rectangle footprint;
	rectangle* boundaries;
	screen* own_screen;
	color own_color;
public:
	//ownscreen and boundaries should be set before using any other members.
	graphic_object( rectangle footprint, color own_color );
	void set_boundaries( rectangle* boundaries );
	void set_own_screen( screen* own_screen );
	rectangle get_footprint();
	void set_footprint( rectangle new_footprint );
	color get_color();
	virtual ~graphic_object() = 0;
	void draw();
	virtual vector_2_u32 move( vector_2_u32 displacement );
	graphic_object* is_selected( vector_2_u32 mouse_pointer );
};
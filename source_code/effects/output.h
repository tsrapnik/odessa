#pragma once

#include "button.h"

class output:
	public graphic_object
{
private:
	double frame;
	bool ready;
public:
	output( rectangle footprint, colour own_colour );
	~output();
	double get_frame();
	void update_frame( double frame );
	void invalidate_frame();
	bool frame_ready();
	void draw_connecting( vector2_i mouse_pointer );
};
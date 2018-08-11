#pragma once

#include "output.h"

class input:
	public graphic_object
{
private:
	output* source;
public:
	input( rectangle footprint, colour own_colour );
	~input();
	void connect_output( output* source );
	double get_frame();
	bool frame_ready();
	void draw_connection();
};
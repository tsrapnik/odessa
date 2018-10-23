#pragma once

#include "output.h"

class input:
	public graphic_object
{
private:
	output* source;
public:
	input( rectangle footprint, color own_color );
	~input();
	void connect_output( output* source );
	f64 get_frame();
	bool frame_ready();
	void draw_connection();
};
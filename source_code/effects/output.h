#pragma once

#include "button.h"

class output:
	public graphic_object
{
private:
	f64 frame;
	bool ready;
public:
	output( rectangle footprint, color own_color );
	~output();
	f64 get_frame();
	void update_frame( f64 frame );
	void invalidate_frame();
	bool frame_ready();
	void draw_connecting( vector_2_u32 mouse_pointer );
};
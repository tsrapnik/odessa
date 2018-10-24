#include "output.h"

output::output( rectangle footprint, color own_color ):
	graphic_object( footprint, own_color )
{}

output::~output()
{
}

f64 output::get_frame()
{
	return frame;
}

void output::update_frame( f64 frame )
{
	this->frame = frame;
	this->ready = true;
}

void output::invalidate_frame()
{
	this->ready = false;
}

bool output::frame_ready()
{
	return ready;
}

void output::draw_connecting( vector_2_u32 mouse_pointer )
{
	vector_2_u32 begin = vector_2_u32( get_footprint().origin.coordinate[0] + get_footprint().size.coordinate[0],
									   get_footprint().origin.coordinate[1] + get_footprint().size.coordinate[1] / 2 );
	own_screen->draw_line( begin, mouse_pointer, own_color );
}

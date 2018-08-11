#include "output.h"

output::output( rectangle footprint, colour own_colour ):
	graphic_object( footprint, own_colour )
{}

output::~output()
{
}

double output::get_frame()
{
	return frame;
}

void output::update_frame( double frame )
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

void output::draw_connecting( vector2_i mouse_pointer )
{
	vector2_i begin = vector2_i( get_footprint().origin.x + get_footprint().size.x,
									   get_footprint().origin.y + get_footprint().size.y / 2 );
	own_screen->draw_line( begin, mouse_pointer, own_colour );
}

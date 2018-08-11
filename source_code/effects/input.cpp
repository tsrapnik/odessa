#include "input.h"

input::input( rectangle footprint, colour own_colour ):
	graphic_object( footprint, own_colour ),
	source( nullptr )
{}

input::~input()
{
}

void input::connect_output( output * source )
{
	this->source = source;
}

double input::get_frame()
{
	if( source )
		return source->get_frame();
	return 0.0f; //if there is no source connected return 0.0f.
}

bool input::frame_ready()
{
	if( source )
		return source->frame_ready();
	return true; //if there is no source connected the frame is always considered ready, the returned frame will always be 0.0f then.
}

void input::draw_connection()
{
	if( source != nullptr )
	{
		vector2_i begin = vector2_i( source->get_footprint().origin.x + source->get_footprint().size.x,
										   source->get_footprint().origin.y + source->get_footprint().size.y / 2 );
		vector2_i end = vector2_i( footprint.origin.x,
										 footprint.origin.y + footprint.size.y / 2 );
		own_screen->draw_line( begin, end, source->get_colour() );
	}
}

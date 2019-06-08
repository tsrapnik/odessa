#include "input.h"

input::input( rectangle footprint, color own_color ):
	drawable( footprint, own_color ),
	source( nullptr )
{}

input::~input()
{
}

void input::connect_output( output * source )
{
	this->source = source;
}

f64 input::get_frame()
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
		vector_2_u32 begin = vector_2_u32( source->get_footprint().origin.coordinate[0] + source->get_footprint().size.coordinate[0],
										   source->get_footprint().origin.coordinate[1] + source->get_footprint().size.coordinate[1] / 2 );
		vector_2_u32 end = vector_2_u32( footprint.origin.coordinate[0],
										 footprint.origin.coordinate[1] + footprint.size.coordinate[1] / 2 );
		own_screen->draw_line( begin, end, source->get_color() );
	}
}

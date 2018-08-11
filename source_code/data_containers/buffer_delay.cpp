#include "buffer_delay.h"

buffer_delay::buffer_delay( int max_length ):
	buffer( max_length ),
	current_position( 0 )
{}

buffer_delay::~buffer_delay()
{}

double buffer_delay::get_previous( int position )
{
	return data[ ( current_position + position ) & max_length_mask ];
}

void buffer_delay::update_current( double value )
{
	data[ current_position ] = value;
	current_position++;
	current_position &= max_length_mask;
}
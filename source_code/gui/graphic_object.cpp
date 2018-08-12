#include "graphic_object.h"

graphic_object::graphic_object( rectangle footprint, colour own_colour )
{
	this->footprint = footprint;
	this->boundaries = nullptr;
	this->own_screen = nullptr;
	this->own_colour = own_colour;
}

void graphic_object::set_boundaries( rectangle* boundaries )
{
	if( footprint.origin.x < 0 )
		footprint.origin.x = 0;
	if( footprint.origin.x + footprint.size.x > boundaries->size.x )
		footprint.size.x = boundaries->size.x - footprint.origin.x;
	if( footprint.origin.y < 0 )
		footprint.origin.y = 0;
	if( footprint.origin.y + footprint.size.y > boundaries->size.y )
		footprint.size.y = boundaries->size.y - footprint.origin.y;

	footprint.origin = vector_2_int::add( footprint.origin, boundaries->origin );
	this->boundaries = boundaries;
}

void graphic_object::set_own_screen( screen* own_screen )
{
	this->own_screen = own_screen;
}

rectangle graphic_object::get_footprint()
{
	return footprint;
}

void graphic_object::set_footprint( rectangle new_footprint )
{
	footprint = new_footprint;
}

colour graphic_object::get_colour()
{
	return own_colour;
}

graphic_object::~graphic_object()
{}

void graphic_object::draw()
{
	for( int x = footprint.origin.x; x < footprint.origin.x + footprint.size.x; x++ )
		for( int y = footprint.origin.y; y < footprint.origin.y + footprint.size.y; y++ )
			own_screen->put_transparent_pixel( vector_2_int( x, y ), own_colour );
}

vector_2_int graphic_object::move( vector_2_int displacement )
{
	if( footprint.origin.x + displacement.x < boundaries->origin.x )
		displacement.x = boundaries->origin.x - footprint.origin.x;
	if( footprint.origin.x + footprint.size.x + displacement.x > boundaries->origin.x + boundaries->size.x )
		displacement.x = boundaries->origin.x + boundaries->size.x - footprint.origin.x - footprint.size.x;
	if( footprint.origin.y + displacement.y < boundaries->origin.y )
		displacement.y = boundaries->origin.y - footprint.origin.y;
	if( footprint.origin.y + footprint.size.y + displacement.y > boundaries->origin.y + boundaries->size.y )
		displacement.y = boundaries->origin.y + boundaries->size.y - footprint.origin.y - footprint.size.y;

	footprint.origin = vector_2_int::add( footprint.origin, displacement );

	return displacement;
}

graphic_object * graphic_object::is_selected( vector_2_int mouse_pointer )
{
	if( mouse_pointer.x >= footprint.origin.x &&
		mouse_pointer.x < footprint.origin.x + footprint.size.x &&
		mouse_pointer.y >= footprint.origin.y &&
		mouse_pointer.y < footprint.origin.y + footprint.size.y )
		return this;
	return nullptr;
}

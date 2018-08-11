#include "vector2_i.h"

vector2_i::vector2_i()
{}

vector2_i::vector2_i( int x, int y )
{
	this->x = x;
	this->y = y;
}

vector2_i::~vector2_i()
{}

vector2_i vector2_i::add( const vector2_i& first, const vector2_i & second )
{
	return vector2_i( first.x + second.x, first.y + second.y );
}

vector2_i vector2_i::subtract( const vector2_i& first, const vector2_i & second )
{
	return vector2_i( first.x - second.x, first.y - second.y );
}

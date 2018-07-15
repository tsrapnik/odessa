#include "vector2I.h"

vector2I::vector2I()
{}

vector2I::vector2I( int x, int y )
{
	this->x = x;
	this->y = y;
}

vector2I::~vector2I()
{}

vector2I vector2I::add( const vector2I& first, const vector2I & second )
{
	return vector2I( first.x + second.x, first.y + second.y );
}

vector2I vector2I::subtract( const vector2I& first, const vector2I & second )
{
	return vector2I( first.x - second.x, first.y - second.y );
}

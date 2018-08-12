#include "vector_2_int.h"

vector_2_int::vector_2_int()
{}

vector_2_int::vector_2_int( int x, int y )
{
	this->x = x;
	this->y = y;
}

vector_2_int::~vector_2_int()
{}

vector_2_int vector_2_int::add( const vector_2_int& first, const vector_2_int & second )
{
	return vector_2_int( first.x + second.x, first.y + second.y );
}

vector_2_int vector_2_int::subtract( const vector_2_int& first, const vector_2_int & second )
{
	return vector_2_int( first.x - second.x, first.y - second.y );
}

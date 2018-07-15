#include "bufferDelay.h"

bufferDelay::bufferDelay( int maxLength ):
	buffer( maxLength ),
	currentPosition( 0 )
{}

bufferDelay::~bufferDelay()
{}

double bufferDelay::getPrevious( int position )
{
	return data[ ( currentPosition + position ) & maxLengthMask ];
}

void bufferDelay::updateCurrent( double value )
{
	data[ currentPosition ] = value;
	currentPosition++;
	currentPosition &= maxLengthMask;
}

#include "bufferFifo.h"

bufferFifo::bufferFifo( int maxLength ):
	buffer( maxLength ),
	beginPosition( 0 ),
	endPosition( 0 ),
	queueLength( 0 )
{}

bufferFifo::~bufferFifo()
{}

void bufferFifo::push( double value )
{
	if( queueLength <= maxLengthMask )
	{
		data[ beginPosition ] = value;
		beginPosition++;
		beginPosition &= maxLengthMask;
		queueLength++;
	}
}

double bufferFifo::pop()
{
	if( queueLength > 0 )
	{
		double value = data[ endPosition ];
		endPosition++;
		endPosition &= maxLengthMask;
		queueLength--;
		return value;
	}
	return 0.0f;
}

int bufferFifo::getQueueLength()
{
	return queueLength;
}

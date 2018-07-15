#include "buffer.h"

buffer::buffer( int maxLength )
{
	if( maxLength <= 0 ) //Make size at least 1.
	{
		maxLengthMask = 1;
		data = new double[ 1 ];
	}
	else //Increase the maxLength to a power of 2 for faster calculation.
	{
		maxLengthMask = 1;
		while( maxLengthMask < maxLength )
		{
			maxLengthMask <<= 1;
		}
		data = new double[ maxLengthMask ];
		maxLengthMask--;
	}
}

buffer::~buffer()
{
	delete data;
}

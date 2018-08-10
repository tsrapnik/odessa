#pragma once

#include "buffer.h"
class bufferDelay:
	public buffer
{
private:
	int currentPosition;
public:
	bufferDelay( int maxLength );
	~bufferDelay();
	double getPrevious( int position );
	void updateCurrent( double value );
};
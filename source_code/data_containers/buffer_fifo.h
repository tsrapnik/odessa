#pragma once

#include "buffer.h"
class bufferFifo:
	public buffer
{
private:
	int beginPosition, endPosition, queueLength;
public:
	bufferFifo( int maxLength );
	~bufferFifo();
	void push( double );
	double pop();
	int getQueueLength();
};
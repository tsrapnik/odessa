#pragma once

#include "buffer.h"
class buffer_fifo:
	public buffer
{
private:
	int begin_position, end_position, queue_length;
public:
	buffer_fifo( int max_length );
	~buffer_fifo();
	void push( double );
	double pop();
	int get_queue_length();
};
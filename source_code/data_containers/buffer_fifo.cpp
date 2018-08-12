#include "buffer_fifo.h"

buffer_fifo::buffer_fifo(int max_length) : buffer(max_length),
										   begin_position(0),
										   end_position(0),
										   queue_length(0)
{
}

buffer_fifo::~buffer_fifo()
{
}

void buffer_fifo::push(double value)
{
	if (queue_length <= max_length_mask)
	{
		data[begin_position] = value;
		begin_position++;
		begin_position &= max_length_mask;
		queue_length++;
	}
}

double buffer_fifo::pop()
{
	if (queue_length > 0)
	{
		double value = data[end_position];
		end_position++;
		end_position &= max_length_mask;
		queue_length--;
		return value;
	}
	return 0.0f;
}

int buffer_fifo::get_queue_length()
{
	return queue_length;
}

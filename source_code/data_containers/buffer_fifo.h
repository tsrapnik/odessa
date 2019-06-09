#pragma once

#include "buffer.h"
class buffer_fifo : public buffer
{
  private:
	u32 begin_position, end_position, queue_length;

  public:
	buffer_fifo(u32 max_length);
	~buffer_fifo();
	void push(f32);
	f32 pop();
	u32 get_queue_length();
};
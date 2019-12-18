#pragma once

#include "buffer.h"

template <typename type>
class buffer_fifo : public buffer<type>
{
    private:
    u32 begin_position, end_position, queue_length;

    public:
    buffer_fifo(u32 max_length);
    ~buffer_fifo();
    void push(type);
    type pop();
    u32 get_queue_length();
};

template <typename type>
buffer_fifo::buffer_fifo(u32 max_length) :
    buffer<type>(max_length),
    begin_position(0),
    end_position(0),
    queue_length(0)
{
}

template <typename type>
buffer_fifo::~buffer_fifo()
{
}

template <typename type>
void buffer_fifo::push(type value)
{
    if(queue_length <= max_length_mask)
    {
        this->data[begin_position] = value;
        begin_position++;
        begin_position &= this->max_length_mask;
        queue_length++;
    }
}

template <typename type>
type buffer_fifo::pop()
{
    if(queue_length > 0)
    {
        type value = this->data[end_position];
        end_position++;
        end_position &= this->max_length_mask;
        queue_length--;
        return value;
    }
    return 0.0f;
}

template <typename type>
u32 buffer_fifo::get_queue_length()
{
    return queue_length;
}
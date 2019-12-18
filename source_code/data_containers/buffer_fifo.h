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

//create a fifo buffer in which you can push new values and pop the oldest
//value in the queue. give the maximum length the fifo can have.
template <typename type>
buffer_fifo<type>::buffer_fifo(u32 max_length) :
    buffer<type>(max_length),
    begin_position(0),
    end_position(0),
    queue_length(0)
{
}

template <typename type>
buffer_fifo<type>::~buffer_fifo()
{
}

//add a new value to the fifo. if the fifo is full, do nothing.
template <typename type>
void buffer_fifo<type>::push(type value)
{
    if(queue_length <= this->max_length_mask)
    {
        this->data[begin_position] = value;
        begin_position++;
        begin_position &= this->max_length_mask;
        queue_length++;
    }
}

//read and remove the oldest value from the fifo. if the fifo is empty
//return the default constructed value of used type.
template <typename type>
type buffer_fifo<type>::pop()
{
    if(queue_length > 0)
    {
        type value = this->data[end_position];
        end_position++;
        end_position &= this->max_length_mask;
        queue_length--;
        return value;
    }
    return type();
}

//get the current amount of elements in the fifo queue.
template <typename type>
u32 buffer_fifo<type>::get_queue_length()
{
    return queue_length;
}
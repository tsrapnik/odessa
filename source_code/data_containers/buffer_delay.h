#pragma once

#include "buffer.h"

template <typename type>
class buffer_delay : public buffer<type>
{
    private:
    u32 current_position;

    public:
    buffer_delay(u32 max_length);

    ~buffer_delay();

    type get_previous(u32 position);
    void update_current(type value);
};

template <typename type>
buffer_delay<type>::buffer_delay(u32 max_length) :
    buffer<type>(max_length),
    current_position(0)
{
}

template <typename type>
buffer_delay<type>::~buffer_delay()
{
}

template <typename type>
type buffer_delay<type>::get_previous(u32 position)
{
    return this->data[(current_position + position) & this->max_length_mask];
}

template <typename type>
void buffer_delay<type>::update_current(type value)
{
    this->data[current_position] = value;
    current_position++;
    current_position &= this->max_length_mask;
}
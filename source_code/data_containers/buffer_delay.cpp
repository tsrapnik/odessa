#include "buffer_delay.h"

buffer_delay::buffer_delay(u32 max_length) :
    buffer(max_length),
    current_position(0)
{
}

buffer_delay::~buffer_delay()
{
}




f32 buffer_delay::get_previous(u32 position)
{
    return data[(current_position + position) & max_length_mask];
}

void buffer_delay::update_current(f32 value)
{
    data[current_position] = value;
    current_position++;
    current_position &= max_length_mask;
}
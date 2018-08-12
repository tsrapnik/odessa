#include "buffer.h"

buffer::buffer(int max_length)
{
    if(max_length <= 0) //make size at least 1.
    {
        max_length_mask = 1;
        data = new double[1];
    }
    else //increase the max_length to a power of 2 for faster calculation.
    {
        max_length_mask = 1;
        while(max_length_mask < max_length)
        {
            max_length_mask <<= 1;
        }
        data = new double[max_length_mask];
        max_length_mask--;
    }
}

buffer::~buffer()
{
    delete data;
}

#pragma once

#include "type_definitions.h"

template <typename type>
class buffer
{
  protected:
	u32 max_length_mask;
	type *data;

  public:
	buffer(u32 max_length);
	virtual ~buffer() = 0;
};

template <typename type>
buffer<type>::buffer(u32 max_length)
{
    if(max_length <= 0) //make size at least 1.
    {
        max_length_mask = 1;
        data = new type[1];
    }
    else //increase the max_length to a power of 2 for faster calculation.
    {
        max_length_mask = 1;
        while(max_length_mask < max_length)
        {
            max_length_mask <<= 1;
        }
        data = new type[max_length_mask];
        max_length_mask--;
    }
}

template <typename type>
buffer<type>::~buffer()
{
    delete data;
}

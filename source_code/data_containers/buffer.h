#pragma once

#include "type_definitions.h"

class buffer
{
  protected:
	u32 max_length_mask;
	f32 *data;

  public:
	buffer(u32 max_length);
	virtual ~buffer() = 0;
};
#pragma once

#include "type_definitions.h"

class buffer
{
  protected:
	u32 max_length_mask;
	f64 *data;

  public:
	buffer(u32 max_length);
	virtual ~buffer() = 0;
};
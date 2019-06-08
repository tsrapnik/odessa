#pragma once

#include "vector.h"

class drawable
{
public:
	virtual void move(vector_2_u32 movement) = 0;
};
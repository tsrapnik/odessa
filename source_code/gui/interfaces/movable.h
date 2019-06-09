#pragma once

#include "vector.h"

class movable
{
public:
	virtual void move(vector_2_f32 movement) = 0;
};
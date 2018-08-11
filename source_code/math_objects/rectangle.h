#pragma once

#include "vector2_i.h"

class rectangle
{
public:
	vector2_i origin, size;
	rectangle( vector2_i origin, vector2_i size );
	rectangle();
	~rectangle();
};
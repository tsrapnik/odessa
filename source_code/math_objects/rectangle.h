#pragma once

#include "vector.h"

class rectangle
{
public:
	vector_2_u32 origin, size;
	rectangle( vector_2_u32 origin, vector_2_u32 size );
	rectangle();
	~rectangle();
};
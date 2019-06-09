#pragma once

#include "vector.h"

//todo: make template for different types (u16, u32, f32...).
class rectangle
{
public:
	vector_2_f32 origin, size;
	rectangle( vector_2_f32 origin, vector_2_f32 size );
	rectangle();
	~rectangle();
};
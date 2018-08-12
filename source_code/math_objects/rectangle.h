#pragma once

#include "vector_2_int.h"

class rectangle
{
public:
	vector_2_int origin, size;
	rectangle( vector_2_int origin, vector_2_int size );
	rectangle();
	~rectangle();
};
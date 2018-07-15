#pragma once

#include "vector2I.h"

class rectangle
{
public:
	vector2I origin, size;
	rectangle( vector2I origin, vector2I size );
	rectangle();
	~rectangle();
};
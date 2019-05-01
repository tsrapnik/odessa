#pragma once

#include "type_definitions.h"

class color
{
public:
	u8 blue, green, red, alpha;
	color();
	color( u8 red, u8 green, u8 blue, u8 alpha );
	~color();
<<<<<<< HEAD
=======
	u32 to_argb_u32();
>>>>>>> include_gpu
};
#include "color.h"



color::color()
{}

color::color( u8 red, u8 green, u8 blue, u8 alpha )
{
	this->red = red;
	this->green = green; 
	this->blue = blue;
	this->alpha = alpha;
}

color::~color()
{}

u32 color::to_argb_u32()
{
	return (static_cast<u32>(alpha) << 24) + (static_cast<u32>(red) << 16) + (static_cast<u32>(green) << 8) + static_cast<u32>(blue);
}
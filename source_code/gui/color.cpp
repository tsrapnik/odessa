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

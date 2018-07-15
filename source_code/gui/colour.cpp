#include "colour.h"



colour::colour()
{}

colour::colour( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha )
{
	this->red = red;
	this->green = green; 
	this->blue = blue;
	this->alpha = alpha;
}


colour::~colour()
{}

#pragma once

class colour
{
public:
	unsigned char blue, green, red, alpha;
	colour();
	colour( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha );
	~colour();
};
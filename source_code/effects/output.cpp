#include "output.h"

output::output( rectangle footprint, colour ownColour ):
	graphicObject( footprint, ownColour )
{}

output::~output()
{
}

double output::getFrame()
{
	return frame;
}

void output::updateFrame( double frame )
{
	this->frame = frame;
	this->ready = true;
}

void output::invalidateFrame()
{
	this->ready = false;
}

bool output::frameReady()
{
	return ready;
}

void output::drawConnecting( vector2I mousePointer )
{
	vector2I begin = vector2I( getFootprint().origin.x + getFootprint().size.x,
									   getFootprint().origin.y + getFootprint().size.y / 2 );
	ownScreen->drawLine( begin, mousePointer, ownColour );
}

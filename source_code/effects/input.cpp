#include "input.h"

input::input( rectangle footprint, colour ownColour ):
	graphicObject( footprint, ownColour ),
	source( nullptr )
{}

input::~input()
{
}

void input::connectOutput( output * source )
{
	this->source = source;
}

double input::getFrame()
{
	if( source )
		return source->getFrame();
	return 0.0f; //If there is no source connected return 0.0f.
}

bool input::frameReady()
{
	if( source )
		return source->frameReady();
	return true; //If there is no source connected the frame is always considered ready, the returned frame will always be 0.0f then.
}

void input::drawConnection()
{
	if( source != nullptr )
	{
		vector2I begin = vector2I( source->getFootprint().origin.x + source->getFootprint().size.x,
										   source->getFootprint().origin.y + source->getFootprint().size.y / 2 );
		vector2I end = vector2I( footprint.origin.x,
										 footprint.origin.y + footprint.size.y / 2 );
		ownScreen->drawLine( begin, end, source->getColour() );
	}
}

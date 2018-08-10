#include "effectSink.h"

void effectSink::process()
{
	*sink = ( signed int )monoInput->getFrame();
}
 effectSink::effectSink( signed int* sink, rectangle footprint, colour ownColour ):
	effect( footprint, ownColour ),
	sink( sink )
{
	monoInput = new input( rectangle( vector2I( 15, 20 ), vector2I( 30, 40 ) ),
						   colour( 0, 0, 255, 255 ) );
	addInput( monoInput );
}
 effectSink::~effectSink()
{
	delete monoInput;
}

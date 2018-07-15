#include "effectSource.h"

void effectSource::process()
{
	monoOutput->updateFrame( ( double )*source );
}

effectSource::effectSource( signed int* source, rectangle footprint, colour ownColour ):
	effect( footprint, ownColour ),
	source( source )
{
	monoOutput = new output( rectangle( vector2I( 15, 20 ), vector2I( 30, 40 ) ),
							 colour( 0, 255, 0, 255 ) );
	addOutput( monoOutput );
}

effectSource::~effectSource()
{
	delete monoOutput;
}

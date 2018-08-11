#include "effect_source.h"

void effect_source::process()
{
	mono_output->update_frame( ( double )*source );
}

effect_source::effect_source( signed int* source, rectangle footprint, colour own_colour ):
	effect( footprint, own_colour ),
	source( source )
{
	mono_output = new output( rectangle( vector2_i( 15, 20 ), vector2_i( 30, 40 ) ),
							 colour( 0, 255, 0, 255 ) );
	add_output( mono_output );
}

effect_source::~effect_source()
{
	delete mono_output;
}

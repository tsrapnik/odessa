#include "effect_sink.h"

void effect_sink::process()
{
	*sink = ( signed int )mono_input->get_frame();
}
 effect_sink::effect_sink( signed int* sink, rectangle footprint, colour own_colour ):
	effect( footprint, own_colour ),
	sink( sink )
{
	mono_input = new input( rectangle( vector2_i( 15, 20 ), vector2_i( 30, 40 ) ),
						   colour( 0, 0, 255, 255 ) );
	add_input( mono_input );
}
 effect_sink::~effect_sink()
{
	delete mono_input;
}

#include "effect_delay.h"

void effect_delay::process()
{
	f64 frame = mono_input->get_frame();
    f64 feedback = feedback_button->get_value();
    f64 wet = wet_button->get_value();
	f64 delay_time = delay_time_button->get_value();

	f64 delay_output = delay.get_previous( -( u32 )delay_time );
	frame = ( 1.0 - wet ) * frame + wet * delay_output;

	f64 delay_input = ( 1.0 - feedback ) * frame + feedback * delay_output;
	delay.update_current( delay_input );

	mono_output->update_frame( frame );
}

effect_delay::effect_delay( rectangle footprint, color own_color ):
	effect( footprint, own_color ),
	delay( 12000 )
{
	mono_input = new input( rectangle( vector_2_u32( 0, 120 ), vector_2_u32( 30, 40 ) ),
						   color( 255, 0, 0, 255 ) );
	mono_output = new output( rectangle( vector_2_u32( 170, 120 ), vector_2_u32( 30, 40 ) ),
						   color( 0, 255, 0, 255 ) );
	feedback_button = new rotary_button( rectangle( vector_2_u32( 80, 80 ), vector_2_u32( 30, 40 ) ),
							   color( 0, 0, 255, 255 ) );
	delay_time_button = new rotary_button( rectangle( vector_2_u32( 80, 130 ), vector_2_u32( 30, 40 ) ),
								 color( 0, 0, 255, 255 ) );
	add_input( mono_input );
	add_output( mono_output );
	add_button( feedback_button );
	add_button( delay_time_button );

    feedback_button->set_value( 0.0 );
    wet_button->set_value( 0.5 );
    delay_time_button->set_value( 1.0 );
}


effect_delay::~effect_delay()
{
	delete mono_input;
	delete mono_output;
	delete feedback_button;
	delete delay_time_button;
}

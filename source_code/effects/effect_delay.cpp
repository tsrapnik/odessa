#include "effect_tsrapnik_delay.h"

void effect_tsrapnik_delay::process()
{
	double frame = mono_input->get_frame();
    double feedback = feedback_button->get_value();
    double wet = wet_button->get_value();
	double delay_time = delay_time_button->get_value();

	double delay_output = delay.get_previous( -( int )delay_time );
	frame = ( 1.0 - wet ) * frame + wet * delay_output;

	double delay_input = ( 1.0 - feedback ) * frame + feedback * delay_output;
	delay.update_current( delay_input );

	mono_output->update_frame( frame );
}

effect_tsrapnik_delay::effect_tsrapnik_delay( rectangle footprint, colour own_colour ):
	effect( footprint, own_colour ),
	delay( 12000 )
{
	mono_input = new input( rectangle( vector2_i( 0, 120 ), vector2_i( 30, 40 ) ),
						   colour( 255, 0, 0, 255 ) );
	mono_output = new output( rectangle( vector2_i( 170, 120 ), vector2_i( 30, 40 ) ),
						   colour( 0, 255, 0, 255 ) );
	feedback_button = new analog_button( rectangle( vector2_i( 80, 80 ), vector2_i( 30, 40 ) ),
							   colour( 0, 0, 255, 255 ) );
	delay_time_button = new analog_button( rectangle( vector2_i( 80, 130 ), vector2_i( 30, 40 ) ),
								 colour( 0, 0, 255, 255 ) );
	add_input( mono_input );
	add_output( mono_output );
	add_button( feedback_button );
	add_button( delay_time_button );

    feedback_button->set_value( 0.0 );
    wet_button->set_value( 0.5 );
    delay_time_button->set_value( 1.0 );
}


effect_tsrapnik_delay::~effect_tsrapnik_delay()
{
	delete mono_input;
	delete mono_output;
	delete feedback_button;
	delete delay_time_button;
}

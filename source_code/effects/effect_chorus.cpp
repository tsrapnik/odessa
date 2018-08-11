#include "effect_tsrapnik_chorus.h"

void effect_tsrapnik_chorus::process()
{
	double frame = mono_input->get_frame();
	double feedback = feedback_button->get_value();
	double wet = wet_button->get_value();

	double delay_output = delay.get_previous( -12000 );
	frame = ( 1.0f - wet ) * frame + wet * delay_output;

	double delay_input = ( 1.0f - feedback ) * frame + feedback * delay_output;
	delay.update_current( delay_input );

	mono_output->update_frame( frame );
}

effect_tsrapnik_chorus::effect_tsrapnik_chorus( rectangle footprint, colour own_colour ):
	effect( footprint, own_colour ),
	delay( 12000 )
{
	mono_input = new input( rectangle( vector2_i( 0, 120 ), vector2_i( 30, 40 ) ),
						   colour( 255, 0, 0, 255 ) );
	mono_output = new output( rectangle( vector2_i( 170, 120 ), vector2_i( 30, 40 ) ),
						   colour( 0, 255, 0, 255 ) );
	feedback_button = new analog_button( rectangle( vector2_i( 80, 80 ), vector2_i( 30, 40 ) ),
							   colour( 0, 0, 255, 255 ) );
	wet_button = new analog_button( rectangle( vector2_i( 80, 130 ), vector2_i( 30, 40 ) ),
								 colour( 0, 0, 255, 255 ) );
	add_input( mono_input );
	add_output( mono_output );
	add_button( feedback_button );
	add_button( wet_button );

	feedback_button->set_value( 0.9f );
	wet_button->set_value( 0.7f );
}


effect_tsrapnik_chorus::~effect_tsrapnik_chorus()
{
	delete mono_input;
	delete mono_output;
	delete feedback_button;
	delete wet_button;
}

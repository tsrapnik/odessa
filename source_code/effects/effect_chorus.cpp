#include "effectTsrapnikChorus.h"

void effectTsrapnikChorus::process()
{
	double frame = monoInput->getFrame();
	double feedback = feedbackButton->getValue();
	double wet = wetButton->getValue();

	double delayOutput = delay.getPrevious( -12000 );
	frame = ( 1.0f - wet ) * frame + wet * delayOutput;

	double delayInput = ( 1.0f - feedback ) * frame + feedback * delayOutput;
	delay.updateCurrent( delayInput );

	monoOutput->updateFrame( frame );
}

effectTsrapnikChorus::effectTsrapnikChorus( rectangle footprint, colour ownColour ):
	effect( footprint, ownColour ),
	delay( 12000 )
{
	monoInput = new input( rectangle( vector2I( 0, 120 ), vector2I( 30, 40 ) ),
						   colour( 255, 0, 0, 255 ) );
	monoOutput = new output( rectangle( vector2I( 170, 120 ), vector2I( 30, 40 ) ),
						   colour( 0, 255, 0, 255 ) );
	feedbackButton = new analogButton( rectangle( vector2I( 80, 80 ), vector2I( 30, 40 ) ),
							   colour( 0, 0, 255, 255 ) );
	wetButton = new analogButton( rectangle( vector2I( 80, 130 ), vector2I( 30, 40 ) ),
								 colour( 0, 0, 255, 255 ) );
	addInput( monoInput );
	addOutput( monoOutput );
	addButton( feedbackButton );
	addButton( wetButton );

	feedbackButton->setValue( 0.9f );
	wetButton->setValue( 0.7f );
}


effectTsrapnikChorus::~effectTsrapnikChorus()
{
	delete monoInput;
	delete monoOutput;
	delete feedbackButton;
	delete wetButton;
}

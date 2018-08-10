#include "effectTsrapnikDelay.h"

void effectTsrapnikDelay::process()
{
	double frame = monoInput->getFrame();
    double feedback = feedbackButton->getValue();
    double wet = wetButton->getValue();
	double delayTime = delayTimeButton->getValue();

	double delayOutput = delay.getPrevious( -( int )delayTime );
	frame = ( 1.0 - wet ) * frame + wet * delayOutput;

	double delayInput = ( 1.0 - feedback ) * frame + feedback * delayOutput;
	delay.updateCurrent( delayInput );

	monoOutput->updateFrame( frame );
}

effectTsrapnikDelay::effectTsrapnikDelay( rectangle footprint, colour ownColour ):
	effect( footprint, ownColour ),
	delay( 12000 )
{
	monoInput = new input( rectangle( vector2I( 0, 120 ), vector2I( 30, 40 ) ),
						   colour( 255, 0, 0, 255 ) );
	monoOutput = new output( rectangle( vector2I( 170, 120 ), vector2I( 30, 40 ) ),
						   colour( 0, 255, 0, 255 ) );
	feedbackButton = new analogButton( rectangle( vector2I( 80, 80 ), vector2I( 30, 40 ) ),
							   colour( 0, 0, 255, 255 ) );
	delayTimeButton = new analogButton( rectangle( vector2I( 80, 130 ), vector2I( 30, 40 ) ),
								 colour( 0, 0, 255, 255 ) );
	addInput( monoInput );
	addOutput( monoOutput );
	addButton( feedbackButton );
	addButton( delayTimeButton );

    feedbackButton->setValue( 0.0 );
    wetButton->setValue( 0.5 );
    delayTimeButton->setValue( 1.0 );
}


effectTsrapnikDelay::~effectTsrapnikDelay()
{
	delete monoInput;
	delete monoOutput;
	delete feedbackButton;
	delete delayTimeButton;
}

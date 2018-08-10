#include "analogButton.h"

analogButton::analogButton( rectangle footprint, colour ownColour ):
	button( footprint, ownColour )
{
	value = 0.0f;
}

analogButton::~analogButton()
{}

double analogButton::getValue()
{
	return value;
}

void analogButton::setValue( double value )
{
	this->value = value;
}

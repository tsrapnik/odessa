#pragma once

#include "button.h"

template <typename type>
class discreteButton:
	public button
{
private:
	type value;
public:
	discreteButton( rectangle footprint, colour ownColour );
	~discreteButton();
	type getValue();
	void setValue( type value );
};

template <typename type>
discreteButton<type>::discreteButton( rectangle footprint, colour ownColour ):
	button( footprint, ownColour )
{}

template <typename type>
discreteButton<type>::~discreteButton()
{}

template <typename type>
type discreteButton<type>::getValue()
{
	return value;
}

template <typename type>
void discreteButton<type>::setValue( type value )
{
	this->value = value;
}
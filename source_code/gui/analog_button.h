#pragma once

#include "button.h"

class analogButton:
	public button
{
private:
	double value;
public:
	analogButton( rectangle footprint, colour ownColour );
	~analogButton();
	double getValue();
	void setValue( double value );
};
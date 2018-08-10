#pragma once

#include "rectangle.h"
#include "colour.h"
#include "screen.h"

class graphicObject
{
protected:
	rectangle footprint;
	rectangle* boundaries;
	screen* ownScreen;
	colour ownColour;
public:
	//Ownscreen and boundaries should be set before using any other members.
	graphicObject( rectangle footprint, colour ownColour );
	void setBoundaries( rectangle* boundaries );
	void setOwnScreen( screen* ownScreen );
	rectangle getFootprint();
	void setFootprint( rectangle newFootprint );
	colour getColour();
	virtual ~graphicObject() = 0;
	void draw();
	virtual vector2I move( vector2I displacement );
	graphicObject* isSelected( vector2I mousePointer );
};
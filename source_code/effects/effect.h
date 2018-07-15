#pragma once

#include "list.h"
#include "listIterator.h"
#include "button.h"
#include "input.h"
#include "output.h"
#include "graphicObject.h"

class effect:
	public graphicObject
{
private:
	list< button* > buttons;
	list< input* > inputs;
	list< output* > outputs;
	virtual void process() = 0;
protected:
	void addButton( button* newButton );
	void addInput( input* newInput );
	void addOutput( output* newOutput );
public:
	effect( rectangle footprint, colour ownColour );
	~effect();
	void setOwnScreen( screen* ownScreen );
	bool update();
	void draw();
	void drawConnections();
	vector2I move( vector2I displacement );
	graphicObject* selectButton( vector2I mousePointer );
	output* selectOutput( vector2I mousePointer );
	input* selectInput( vector2I mousePointer );
};
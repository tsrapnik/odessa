#pragma once

#include "list.h"
#include "listIterator.h"
#include "effect.h"
#include "screen.h"

class signalPath
{
private:
	screen* ownScreen;
	list< effect* > effects;

public:
	signalPath( screen* ownScreen );
	~signalPath();
	void update();
	void draw();
	void addEffect( effect* newEffect );
	effect* selectEffect( vector2I mousePointer );
	graphicObject* selectButton( vector2I mousePointer );
	output* selectOutput( vector2I mousePointer );
	input* selectInput( vector2I mousePointer );
};
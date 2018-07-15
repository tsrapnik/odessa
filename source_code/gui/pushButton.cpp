#include "pushButton.h"

pushButton::pushButton( rectangle footprint, colour ownColour ):
	button( footprint, ownColour )
{
}

pushButton::~pushButton()
{
}

void pushButton::setPushed()
{
}

///<summary>
///Check if a positive edge was triggered, reset after read.
///</summary>
bool pushButton::checkPushed()
{
	bool temp = pushed;
	pushed = false;
	return temp;
}

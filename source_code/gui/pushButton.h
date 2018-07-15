#pragma once

#include "button.h"
class pushButton:
	public button
{
private:
	bool pushed;
public:
	pushButton( rectangle footprint, colour ownColour );
	~pushButton();
	void setPushed();
	bool checkPushed();
};
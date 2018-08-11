#pragma once

#include "list.h"
#include "list_iterator.h"
#include "effect.h"
#include "screen.h"

class signal_path
{
private:
	screen* own_screen;
	list< effect* > effects;

public:
	signal_path( screen* own_screen );
	~signal_path();
	void update();
	void draw();
	void add_effect( effect* new_effect );
	effect* select_effect( vector2_i mouse_pointer );
	graphic_object* select_button( vector2_i mouse_pointer );
	output* select_output( vector2_i mouse_pointer );
	input* select_input( vector2_i mouse_pointer );
};
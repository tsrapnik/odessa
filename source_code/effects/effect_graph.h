#pragma once

#include "list.h"
#include "list_iterator.h"
#include "effect.h"
#include "screen.h"

class effect_graph
{
private:
	screen* own_screen;
	list< effect* > effects;

public:
	effect_graph( screen* own_screen );
	~effect_graph();
	void update();
	void draw();
	void add_effect( effect* new_effect );
	effect* select_effect( vector_2_u32 mouse_pointer );
	drawable* select_button( vector_2_u32 mouse_pointer );
	output* select_output( vector_2_u32 mouse_pointer );
	input* select_input( vector_2_u32 mouse_pointer );
};
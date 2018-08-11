#include "signal_path.h"

signal_path::signal_path( screen* own_screen )
{
	this->own_screen = own_screen;
}

signal_path::~signal_path()
{}

void signal_path::update()
{
	list_iterator< effect* > current_effect( &effects );
	for( current_effect.to_first(); !current_effect.at_end(); current_effect++ )
	{
		current_effect.get_data_copy()->update();
	}
}

void signal_path::draw()
{
	own_screen->clear( colour( 10, 20, 125, 255 ) );

	list_iterator< effect* > current_effect( &effects );
	for( current_effect.to_first(); !current_effect.at_end(); current_effect++ )
	{
		current_effect.get_data_copy()->draw();
	}
	
	for( current_effect.to_first(); !current_effect.at_end(); current_effect++ )
	{
		current_effect.get_data_copy()->draw_connections();
	}
}

void signal_path::add_effect( effect* new_effect )
{
	new_effect->set_boundaries( own_screen->get_footprint() );
	new_effect->set_own_screen( own_screen );
	effects.append_copy( new_effect );
}

effect* signal_path::select_effect( vector2_i mouse_pointer )
{
	list_iterator< effect* > current_effect( &effects );
	for( current_effect.to_first(); !current_effect.at_end(); current_effect++ )
	{
		effect* selection = ( effect* )current_effect.get_data_copy()->is_selected( mouse_pointer );
		if( selection != nullptr )
		{
			return selection;
		}
	}

	return nullptr;
}

graphic_object * signal_path::select_button( vector2_i mouse_pointer )
{
	list_iterator< effect* > current_effect( &effects );
	for( current_effect.to_first(); !current_effect.at_end(); current_effect++ )
	{
		graphic_object* selection = current_effect.get_data_copy()->select_button( mouse_pointer );
		if( selection != nullptr )
		{
			return selection;
		}
	}

	return nullptr;
}

output * signal_path::select_output( vector2_i mouse_pointer )
{
	list_iterator< effect* > current_effect( &effects );
	for( current_effect.to_first(); !current_effect.at_end(); current_effect++ )
	{
		output* selection = nullptr;
		selection = current_effect.get_data_copy()->select_output( mouse_pointer );
		if( selection != nullptr )
		{
			return selection;
		}
	}

	return nullptr;
}

input* signal_path::select_input( vector2_i mouse_pointer )
{
	list_iterator< effect* > current_effect( &effects );
	for( current_effect.to_first(); !current_effect.at_end(); current_effect++ )
	{
		input* selection = nullptr;
		selection = current_effect.get_data_copy()->select_input( mouse_pointer );
		if( selection != nullptr )
		{
			return selection;
		}
	}

	return nullptr;
}
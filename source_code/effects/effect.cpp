#include "effect.h"

#include "analog_button.h"

effect::effect( rectangle footprint, colour own_colour ) :
	graphic_object( footprint, own_colour )
{}

effect::~effect()
{}

void effect::set_own_screen( screen* own_screen )
{
	graphic_object::set_own_screen( own_screen );

	list_iterator< input* > inputs_iterator( &inputs );
	for( inputs_iterator.to_first(); !inputs_iterator.at_end(); inputs_iterator++ )
		inputs_iterator.get_data_copy()->set_own_screen( own_screen );

	list_iterator< output* > outputs_iterator( &outputs );
	for( outputs_iterator.to_first(); !outputs_iterator.at_end(); outputs_iterator++ )
		outputs_iterator.get_data_copy()->set_own_screen( own_screen );

	list_iterator< button* > buttons_iterator( &buttons );
	for( buttons_iterator.to_first(); !buttons_iterator.at_end(); buttons_iterator++ )
		buttons_iterator.get_data_copy()->set_own_screen( own_screen );
}
//commit1
void effect::add_button( button* new_button )
{
	new_button->set_boundaries( &this->footprint );
	buttons.append_copy( new_button );
}

void effect::add_input( input* new_input )
{
	new_input->set_boundaries( &this->footprint );
	inputs.append_copy( new_input );
}

void effect::add_output( output* new_output )
{
	new_output->set_boundaries( &this->footprint );
	outputs.append_copy( new_output );
}

//calculates all outputs and marks them as ready. returns false when the inputs weren't ready yet.
bool effect::update()
{
	list_iterator< input* > inputs_iterator( &inputs );
	for( inputs_iterator.to_first(); !inputs_iterator.at_end(); inputs_iterator++ )
	{
		if( !inputs_iterator.get_data_copy()->frame_ready() )
			return false; //return false when one of the inputs is not ready, so no processing is done.
	}

	process();

	return true;
}

void effect::draw()
{
	graphic_object::draw();

	list_iterator< input* > inputs_iterator( &inputs );
	for( inputs_iterator.to_first(); !inputs_iterator.at_end(); inputs_iterator++ )
		inputs_iterator.get_data_copy()->draw();

	list_iterator< output* > outputs_iterator( &outputs );
	for( outputs_iterator.to_first(); !outputs_iterator.at_end(); outputs_iterator++ )
		outputs_iterator.get_data_copy()->draw();

	list_iterator< button* > buttons_iterator( &buttons );
	for( buttons_iterator.to_first(); !buttons_iterator.at_end(); buttons_iterator++ )
		buttons_iterator.get_data_copy()->draw();
}

void effect::draw_connections()
{
	list_iterator< input* > inputs_iterator( &inputs );
	for( inputs_iterator.to_first(); !inputs_iterator.at_end(); inputs_iterator++ )
		inputs_iterator.get_data_copy()->draw_connection();
}

vector_2_int effect::move( vector_2_int displacement )
{
	displacement = graphic_object::move( displacement );

	list_iterator< input* > inputs_iterator( &inputs );
	for( inputs_iterator.to_first(); !inputs_iterator.at_end(); inputs_iterator++ )
		inputs_iterator.get_data_copy()->move( displacement );

	list_iterator< output* > outputs_iterator( &outputs );
	for( outputs_iterator.to_first(); !outputs_iterator.at_end(); outputs_iterator++ )
		outputs_iterator.get_data_copy()->move( displacement );

	list_iterator< button* > buttons_iterator( &buttons );
	for( buttons_iterator.to_first(); !buttons_iterator.at_end(); buttons_iterator++ )
		buttons_iterator.get_data_copy()->move( displacement );

	return displacement;
}

graphic_object * effect::select_button( vector_2_int mouse_pointer )
{
	list_iterator< input* > inputs_iterator( &inputs );
	for( inputs_iterator.to_first(); !inputs_iterator.at_end(); inputs_iterator++ )
	{
		graphic_object* selection = inputs_iterator.get_data_copy()->is_selected( mouse_pointer );
		if( selection != nullptr )
			return selection;
	}

	list_iterator< output* > outputs_iterator( &outputs );
	for( outputs_iterator.to_first(); !outputs_iterator.at_end(); outputs_iterator++ )
	{
		graphic_object* selection = outputs_iterator.get_data_copy()->is_selected( mouse_pointer );
		if( selection != nullptr )
			return selection;
	}

	list_iterator< button* > buttons_iterator( &buttons );
	for( buttons_iterator.to_first(); !buttons_iterator.at_end(); buttons_iterator++ )
	{
		graphic_object* selection = buttons_iterator.get_data_copy()->is_selected( mouse_pointer );
		if( selection != nullptr )
			return selection;
	}

	return nullptr;
}

output* effect::select_output( vector_2_int mouse_pointer )
{
	list_iterator< output* > outputs_iterator( &outputs );
	for( outputs_iterator.to_first(); !outputs_iterator.at_end(); outputs_iterator++ )
	{
		output* selection = ( output* )outputs_iterator.get_data_copy()->is_selected( mouse_pointer );
		if( selection != nullptr )
			return selection;
	}

	return nullptr;
}

input* effect::select_input( vector_2_int mouse_pointer )
{
	list_iterator< input* > inputs_iterator( &inputs );
	for( inputs_iterator.to_first(); !inputs_iterator.at_end(); inputs_iterator++ )
	{
		input* selection = ( input* )inputs_iterator.get_data_copy()->is_selected( mouse_pointer );
		if( selection != nullptr )
			return selection;
	}

	return nullptr;
}

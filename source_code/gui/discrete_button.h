#pragma once

#include "button.h"

template <typename type>
class discrete_button:
	public button
{
private:
	type value;
public:
	discrete_button( rectangle footprint, colour own_colour );
	~discrete_button();
	type get_value();
	void set_value( type value );
};

template <typename type>
discrete_button<type>::discrete_button( rectangle footprint, colour own_colour ):
	button( footprint, own_colour )
{}

template <typename type>
discrete_button<type>::~discrete_button()
{}

template <typename type>
type discrete_button<type>::get_value()
{
	return value;
}

template <typename type>
void discrete_button<type>::set_value( type value )
{
	this->value = value;
}
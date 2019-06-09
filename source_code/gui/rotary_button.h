#pragma once

#include "button.h"

template <typename type>
class rotary_button : public button
{
    private:
    type value;

    public:
    rotary_button(rectangle bounding_box, color the_color);
    virtual ~rotary_button();
    type get_value();
    void set_value(type value);
};

template <typename type>
rotary_button<type>::rotary_button(rectangle bounding_box, color the_color) :
    button(bounding_box, the_color)
{
}

template <typename type>
rotary_button<type>::~rotary_button()
{
}

template <typename type>
type rotary_button<type>::get_value()
{
    return value;
}

template <typename type>
void rotary_button<type>::set_value(type value)
{
    this->value = value;
}
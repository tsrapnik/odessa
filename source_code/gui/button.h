#pragma once

#include "drawable.h"

class button : public drawable
{
    public:
    button(rectangle footprint, color own_color);
    ~button();
};
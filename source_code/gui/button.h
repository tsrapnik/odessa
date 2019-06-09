#pragma once

#include "graphic.h"
#include "rectangle.h"

class button
{
    private:
        graphic* the_graphic;
    public:
    button(rectangle footprint, color own_color);
    virtual ~button();
};
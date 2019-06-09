#pragma once

#include "drawable.h"
#include "movable.h"
#include "selectable.h"
#include "rectangle.h"

class graphic : public drawable, public selectable, public movable
{
    public:
    virtual rectangle get_bounding_box() = 0;
    virtual ~graphic(){};
};
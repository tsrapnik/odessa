#pragma once

#include "drawable.h"
#include "movable.h"
#include "selectable.h"

class graphic: public drawable, public selectable, public movable
{
};
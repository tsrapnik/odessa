#include "rectangle.h"

rectangle::rectangle(vector_2_f32 origin, vector_2_f32 size)
{
    this->origin = origin;
    this->size = size;
}

rectangle::rectangle()
{
}

rectangle::~rectangle()
{
}

vector_2_f32 rectangle::get_center()
{
    return vector_2_f32::sum(this->origin,
                             vector_2_f32::scalar_product(0.5f, this->size));
}
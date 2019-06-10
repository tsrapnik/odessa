#include "effect_source.h"

void effect_source::process()
{
    the_output->update_frame(static_cast<f32>(*source));
}

effect_source::effect_source(i32* source, rectangle bounding_box, color the_color) :
    effect(bounding_box, the_color),
    source(source)
{
    the_output = new output(rectangle(vector_2_f32(15, 20), vector_2_f32(30, 40)),
                             color(0, 255, 0, 255));
    add_output(the_output);
}

effect_source::~effect_source()
{
    delete the_output;
}

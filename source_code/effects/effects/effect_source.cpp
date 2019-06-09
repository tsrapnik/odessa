#include "effect_source.h"

void effect_source::process()
{
    mono_output->update_frame(static_cast<f32>(*source));
}

effect_source::effect_source(i32* source, rectangle footprint, color own_color) :
    effect(footprint, own_color),
    source(source)
{
    mono_output = new output(rectangle(vector_2_f32(15, 20), vector_2_f32(30, 40)),
                             color(0, 255, 0, 255));
    add_output(mono_output);
}

effect_source::~effect_source()
{
    delete mono_output;
}

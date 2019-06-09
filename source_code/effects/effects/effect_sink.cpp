#include "effect_sink.h"

void effect_sink::process()
{
    *sink = static_cast<i32>(mono_input->get_frame());
}
effect_sink::effect_sink(i32* sink, rectangle footprint, color own_color) :
    effect(footprint, own_color),
    sink(sink)
{
    mono_input = new input(rectangle(vector_2_f32(15, 20), vector_2_f32(30, 40)),
                           color(0, 0, 255, 255));
    add_input(mono_input);
}
effect_sink::~effect_sink()
{
    delete mono_input;
}

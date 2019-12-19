#include "effect_sink.h"

void effect_sink::process()
{
    *sink = static_cast<i32>(the_input->get_frame());
}
effect_sink::effect_sink(i32* sink, rectangle bounding_box, color the_color) :
    effect(bounding_box, the_color),
    sink(sink)
{
    the_input = new input(rectangle(vector_2_f32::sum(get_bounding_box().origin, vector_2_f32(15, 20)), vector_2_f32(30, 40)),
                          color(255, 0, 0, 255));
    add_input(the_input);
}
effect_sink::~effect_sink()
{
    delete the_input;
}

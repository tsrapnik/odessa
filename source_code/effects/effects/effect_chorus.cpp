#include "effect_chorus.h"

void effect_chorus::process()
{
    f32 frame = mono_input->get_frame();
    f32 feedback = feedback_button->get_value();
    f32 wet = wet_button->get_value();

    f32 delay_output = delay.get_previous(-12000);
    frame = (1.0f - wet) * frame + wet * delay_output;

    f32 delay_input = (1.0f - feedback) * frame + feedback * delay_output;
    delay.update_current(delay_input);

    mono_output->update_frame(frame);
}

effect_chorus::effect_chorus(rectangle footprint, color own_color) :
    effect(footprint, own_color),
    delay(12000)
{
    mono_input = new input(rectangle(vector_2_f32(0, 120), vector_2_f32(30, 40)),
                           color(255, 0, 0, 255));
    mono_output = new output(rectangle(vector_2_f32(170, 120), vector_2_f32(30, 40)),
                             color(0, 255, 0, 255));
    feedback_button = new rotary_button<f32>(rectangle(vector_2_f32(80, 80), vector_2_f32(30, 40)),
                                        color(0, 0, 255, 255));
    wet_button = new rotary_button<f32>(rectangle(vector_2_f32(80, 130), vector_2_f32(30, 40)),
                                   color(0, 0, 255, 255));
    add_input(mono_input);
    add_output(mono_output);
    add_button(feedback_button);
    add_button(wet_button);

    feedback_button->set_value(0.9f);
    wet_button->set_value(0.7f);
}

effect_chorus::~effect_chorus()
{
    delete mono_input;
    delete mono_output;
    delete feedback_button;
    delete wet_button;
}

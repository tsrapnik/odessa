#pragma once

#include "effect.h"

class effect_source : public effect
{
    private:
    ///points to the actual 32 bit value that is received from the audio intput, e.g. adc.
    i32* source;

    output* the_output;

    void process() override;

    public:
    effect_source(i32* source, rectangle bounding_box, color the_color);
    ~effect_source();
};
#pragma once

#include "effect.h"
#include "rotary_button.h"
#include "buffer_delay.h"

class effect_chorus : public effect
{
  private:
	input *the_input;
	output *the_output;
	rotary_button<f32> *feedback_button, *wet_button;

	buffer_delay delay;

	void process() override;

  public:
	effect_chorus(rectangle bounding_box, color the_color);
	~effect_chorus();
};
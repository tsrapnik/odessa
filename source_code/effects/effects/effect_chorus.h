#pragma once

#include "effect.h"
#include "rotary_button.h"
#include "buffer_delay.h"

class effect_chorus : public effect
{
  private:
	input *mono_input;
	output *mono_output;
	rotary_button<f32> *feedback_button, *wet_button;

	buffer_delay delay;

	void process();

  public:
	effect_chorus(rectangle footprint, color own_color);
	~effect_chorus();
};
#pragma once
#include "type_definitions.h"

class font
{
	public:
	static constexpr u32 letter_count = 128;
	static constexpr u32 letter_height = 18;
	static constexpr u32 letter_width = 11;
	static const u8 monospace[font::letter_count][font::letter_height][font::letter_width];
};
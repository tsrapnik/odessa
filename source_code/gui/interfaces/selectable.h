#pragma once

class selectable
{
public:
	virtual bool is_selected(vector_2_f32 mouse_position) = 0;
	virtual ~selectable(){}
};
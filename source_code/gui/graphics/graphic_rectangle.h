#pragma once

#include "graphic.h"
#include "rectangle.h"

class graphic_rectangle: public graphic
{
    protected:
    rectangle bounding_box;
    color the_color;

    public:
    graphic_rectangle(rectangle bounding_box, color the_color);
    virtual ~graphic_rectangle();

    void set_bounding_box(rectangle new_bounding_box);
    color get_color();
    void set_color(color new_color);

	void draw(scene_2d& scene) override;
	bool is_selected(vector_2_f32 mouse_position) override;
	void move(vector_2_f32 displacement) override;
    rectangle get_bounding_box() override;
};
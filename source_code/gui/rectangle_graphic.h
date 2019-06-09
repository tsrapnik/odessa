#pragma once

#include "graphic.h"
#include "rectangle.h"

class rectangle_graphic: public graphic
{
    protected:
    rectangle footprint;
    color own_color;

    public:
    rectangle_graphic(rectangle footprint, color own_color);
    virtual ~rectangle_graphic();

    rectangle get_footprint();
    void set_footprint(rectangle new_footprint);
    color get_color();
    void set_color(color new_color);

	void draw(scene_2d& scene) override;
	bool is_selected(vector_2_f32 position) override;
	void move(vector_2_f32 movement) override;
};
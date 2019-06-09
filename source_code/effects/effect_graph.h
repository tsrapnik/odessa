#pragma once

#include "effect.h"
#include "list.h"
#include "list_iterator.h"

class effect_graph
{
    private:
    list<effect*> effects;

    public:
    effect_graph();
    ~effect_graph();
    void process();
    void draw(scene_2d& scene);
    void add_effect(effect* new_effect);
    effect* get_selected_effect(vector_2_f32 mouse_position);
};
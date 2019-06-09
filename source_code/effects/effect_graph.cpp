#include "effect_graph.h"

effect_graph::effect_graph()
{
}

effect_graph::~effect_graph()
{
}

void effect_graph::process()
{
    list<effect*> processed_effects;
    list_iterator<effect*> current_effect(this->effects);
    //each time a input is connected to an output, it should be checked this does not
    //create a direct feedback loop (input of effect depends on it own output). otherwise
    //the process function cannot finish correctly.
    while(!this->effects.is_empty())
    {
        for(current_effect.to_first(); !current_effect.at_end(); current_effect++)
        {
            if(current_effect.get_data_copy()->process_with_success())
            {
                processed_effects.append_copy(current_effect.get_data_copy());
                current_effect.delete_current();
                break;
            }
            //this line should never get reached. if reached we return, to prevent an
            //infinite loop.
            //todo: add error log to this line.
            return;
        }
    }
    this->effects = processed_effects;
}

void effect_graph::draw(scene_2d& scene)
{
    list_iterator<effect*> current_effect(effects);
    for(current_effect.to_first(); !current_effect.at_end(); current_effect++)
    {
        current_effect.get_data_copy()->draw(scene);
    }

    for(current_effect.to_first(); !current_effect.at_end(); current_effect++)
    {
        current_effect.get_data_copy()->draw_connections(scene);
    }
}

void effect_graph::add_effect(effect* new_effect)
{
    effects.append_copy(new_effect);
}

effect* effect_graph::get_selected_effect(vector_2_f32 mouse_position)
{
    list_iterator<effect*> current_effect(effects);
    for(current_effect.to_first(); !current_effect.at_end(); current_effect++)
    {
        if(current_effect.get_data_copy()->is_selected(mouse_position))
            return current_effect.get_data_copy();
    }
    return nullptr;
}
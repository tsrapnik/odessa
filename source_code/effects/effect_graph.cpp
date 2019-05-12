#include "effect_graph.h"

effect_graph::effect_graph(screen* own_screen)
{
    this->own_screen = own_screen;
}

effect_graph::~effect_graph()
{
}

void effect_graph::update()
{
    list_iterator<effect*> current_effect(effects);
    for(current_effect.to_first(); !current_effect.at_end(); current_effect++)
    {
        current_effect.get_data_copy()->update();
    }
}

void effect_graph::draw()
{
    own_screen->clear(color(10, 20, 125, 255));

    list_iterator<effect*> current_effect(effects);
    for(current_effect.to_first(); !current_effect.at_end(); current_effect++)
    {
        current_effect.get_data_copy()->draw();
    }

    for(current_effect.to_first(); !current_effect.at_end(); current_effect++)
    {
        current_effect.get_data_copy()->draw_connections();
    }
}

void effect_graph::add_effect(effect* new_effect)
{
    new_effect->set_boundaries(own_screen->get_footprint());
    new_effect->set_own_screen(own_screen);
    effects.append_copy(new_effect);
}

effect* effect_graph::select_effect(vector_2_u32 mouse_pointer)
{
    list_iterator<effect*> current_effect(effects);
    for(current_effect.to_first(); !current_effect.at_end(); current_effect++)
    {
        effect* selection = (effect*)current_effect.get_data_copy()->is_selected(mouse_pointer);
        if(selection != nullptr)
        {
            return selection;
        }
    }

    return nullptr;
}

graphic_object* effect_graph::select_button(vector_2_u32 mouse_pointer)
{
    list_iterator<effect*> current_effect(effects);
    for(current_effect.to_first(); !current_effect.at_end(); current_effect++)
    {
        graphic_object* selection = current_effect.get_data_copy()->select_button(mouse_pointer);
        if(selection != nullptr)
        {
            return selection;
        }
    }

    return nullptr;
}

output* effect_graph::select_output(vector_2_u32 mouse_pointer)
{
    list_iterator<effect*> current_effect(effects);
    for(current_effect.to_first(); !current_effect.at_end(); current_effect++)
    {
        output* selection = nullptr;
        selection = current_effect.get_data_copy()->select_output(mouse_pointer);
        if(selection != nullptr)
        {
            return selection;
        }
    }

    return nullptr;
}

input* effect_graph::select_input(vector_2_u32 mouse_pointer)
{
    list_iterator<effect*> current_effect(effects);
    for(current_effect.to_first(); !current_effect.at_end(); current_effect++)
    {
        input* selection = nullptr;
        selection = current_effect.get_data_copy()->select_input(mouse_pointer);
        if(selection != nullptr)
        {
            return selection;
        }
    }

    return nullptr;
}
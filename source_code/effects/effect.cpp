#include "effect.h"

#include "rotary_button.h"

effect::effect(rectangle footprint, color own_color)
{
}

effect::~effect()
{
}

void effect::add_button(button* new_button)
{
    buttons.append_copy(new_button);
}

void effect::add_input(input* new_input)
{
    inputs.append_copy(new_input);
}

void effect::add_output(output* new_output)
{
    outputs.append_copy(new_output);
}

//process all outputs and marks them as ready. returns false when the inputs weren't ready yet.
bool effect::process_with_success()
{
    list_iterator<input*> inputs_iterator(inputs);
    for(inputs_iterator.to_first(); !inputs_iterator.at_end(); inputs_iterator++)
    {
        if(!inputs_iterator.get_data_copy()->frame_ready())
            //return false when one of the inputs is not ready, so no processing is done.
            return false;
    }

    process();

    return true;
}

void effect::draw(scene_2d& scene)
{
    the_graphic->draw(scene);

    list_iterator<input*> inputs_iterator(inputs);
    for(inputs_iterator.to_first(); !inputs_iterator.at_end(); inputs_iterator++)
        inputs_iterator.get_data_copy()->draw(scene);

    list_iterator<output*> outputs_iterator(outputs);
    for(outputs_iterator.to_first(); !outputs_iterator.at_end(); outputs_iterator++)
        outputs_iterator.get_data_copy()->draw(scene);

    list_iterator<button*> buttons_iterator(buttons);
    for(buttons_iterator.to_first(); !buttons_iterator.at_end(); buttons_iterator++)
        buttons_iterator.get_data_copy()->draw(scene);
}

void effect::draw_connections(scene_2d& scene)
{
    list_iterator<input*> inputs_iterator(inputs);
    for(inputs_iterator.to_first(); !inputs_iterator.at_end(); inputs_iterator++)
        inputs_iterator.get_data_copy()->draw_connection(scene);
}

bool effect::is_selected(vector_2_f32 mouse_position)
{
    return this->the_graphic->is_selected(mouse_position);
}

void effect::move(vector_2_f32 displacement)
{
    the_graphic->move(displacement);

    list_iterator<input*> inputs_iterator(inputs);
    for(inputs_iterator.to_first(); !inputs_iterator.at_end(); inputs_iterator++)
        inputs_iterator.get_data_copy()->move(displacement);

    list_iterator<output*> outputs_iterator(outputs);
    for(outputs_iterator.to_first(); !outputs_iterator.at_end(); outputs_iterator++)
        outputs_iterator.get_data_copy()->move(displacement);

    list_iterator<button*> buttons_iterator(buttons);
    for(buttons_iterator.to_first(); !buttons_iterator.at_end(); buttons_iterator++)
        buttons_iterator.get_data_copy()->move(displacement);
}

button* effect::get_selected_button(vector_2_f32 mouse_position)
{
    list_iterator<button*> current_button(this->buttons);
    for(current_button.to_first(); !current_button.at_end(); current_button++)
    {
        if(current_button.get_data_copy()->is_selected(mouse_position))
            return current_button.get_data_copy();
    }
    return nullptr;
}

output* effect::get_selected_output(vector_2_f32 mouse_position)
{
    list_iterator<output*> current_output(this->outputs);
    for(current_output.to_first(); !current_output.at_end(); current_output++)
    {
        if(current_output.get_data_copy()->is_selected(mouse_position))
            return current_output.get_data_copy();
    }
    return nullptr;
}

input* effect::get_selected_input(vector_2_f32 mouse_position)
{
    list_iterator<input*> current_input(this->inputs);
    for(current_input.to_first(); !current_input.at_end(); current_input++)
    {
        if(current_input.get_data_copy()->is_selected(mouse_position))
            return current_input.get_data_copy();
    }
    return nullptr;
}
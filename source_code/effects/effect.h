#pragma once

#include "button.h"
#include "input.h"
#include "output.h"
#include "list.h"
#include "list_iterator.h"

class effect
{
    private:
    graphic* the_graphic;
    list<button*> buttons;
    list<input*> inputs;
    list<output*> outputs;
    virtual void process() = 0;

    protected:
    void add_button(button* new_button);
    void add_input(input* new_input);
    void add_output(output* new_output);

    public:
    effect(rectangle footprint, color own_color);
    ~effect();
    bool update();
    void draw(scene_2d scene);
    void draw_connections(scene_2d scene);
    vector_2_u32 move(vector_2_u32 displacement);
    drawable* get_selected_button(vector_2_u32 mouse_pointer);
    output* get_selected_output(vector_2_u32 mouse_pointer);
    input* get_selected_input(vector_2_u32 mouse_pointer);
};
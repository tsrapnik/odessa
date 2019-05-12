#pragma once

#include "button.h"
#include "graphic_object.h"
#include "input.h"
#include "list.h"
#include "list_iterator.h"
#include "output.h"

class effect : public graphic_object
{
    private:
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
    void set_own_screen(screen* own_screen);
    bool update();
    void draw();
    void draw_connections();
    vector_2_u32 move(vector_2_u32 displacement);
    graphic_object* select_button(vector_2_u32 mouse_pointer);
    output* select_output(vector_2_u32 mouse_pointer);
    input* select_input(vector_2_u32 mouse_pointer);
};
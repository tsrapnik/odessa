#pragma once

#include "button.h"
class push_button : public button
{
    private:
    bool pushed;

    public:
    push_button(rectangle footprint, color own_color);
    ~push_button();
    void set_pushed();
    bool check_pushed();
};
#pragma once

#include "colour.h"
#include "rectangle.h"
#include "type_definitions.h"

class screen
{
    private:
    rectangle footprint;
    colour* buffer;

    public:
    screen(vector_2_int size, colour* buffer);
    ~screen();
    char* get_buffer();
    rectangle* get_footprint();
    void clear(colour back_ground);
    colour get_pixel(vector_2_int position);
    void put_transparent_pixel(vector_2_int position, colour own_colour);
    void put_opaque_pixel(vector_2_int position, colour own_colour);
    void draw_line(vector_2_int begin, vector_2_int end, colour own_colour);
    void draw_text(const char* text, vector_2_int position);
};

inline colour screen::get_pixel(vector_2_int position)
{
    return buffer[position.y * footprint.size.x + position.x];
}

inline void screen::put_transparent_pixel(vector_2_int position, colour own_colour)
{
    int alpha = own_colour.alpha;
    int anti_alpha = 255 - alpha;

    colour this_colour = buffer[position.y * footprint.size.x + position.x];

    this_colour.red = (unsigned char)((this_colour.red * anti_alpha + own_colour.red * alpha) >> 8);
    this_colour.green = (unsigned char)((this_colour.green * anti_alpha + own_colour.green * alpha) >> 8);
    this_colour.blue = (unsigned char)((this_colour.blue * anti_alpha + own_colour.blue * alpha) >> 8);

    buffer[position.y * footprint.size.x + position.x] = this_colour;
}

inline void screen::put_opaque_pixel(vector_2_int position, colour own_colour)
{
    buffer[position.y * footprint.size.x + position.x] = own_colour;
}
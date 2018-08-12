#pragma once

#include "colour.h"
#include "rectangle.h"
#include "type_definitions.h"

class screen
{
    private:
    //tmp
    //only 32 bit colours supported.
    //this struct is shared between g_p_u and a_r_m side
    // must be 16 byte aligned in memory
    struct bcm2835_frame_buffer_info
    {
        u32 width; // physical width of display in pixel
        u32 height; // physical height of display in pixel
        u32 virt_width; // always as physical width so far
        u32 virt_height; // always as physical height so far
        u32 pitch; // should be init with 0
        u32 depth; // number of bits per pixel
        u32 offset_x; // normally zero
        u32 offset_y; // normally zero
        u32 buffer_ptr; // address of frame buffer (init with 0, set by g_p_u)
        u32 buffer_size; // size of frame buffer (init with 0, set by g_p_u)
    }
    __attribute__((packed));

    rectangle footprint;
    colour* buffer;

    public:
    screen(u32 x_size, u32 y_size);
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
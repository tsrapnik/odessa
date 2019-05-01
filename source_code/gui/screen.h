#pragma once

#include "color.h"
#include "rectangle.h"
#include "type_definitions.h"

class screen
{
    private:
    rectangle footprint;
    color* displaybuffer;
<<<<<<< HEAD
    color backbuffer[800*480];
=======
>>>>>>> include_gpu

    public:
    screen(vector_2_u32 size, color* displaybuffer);
    ~screen();
    rectangle* get_footprint();
<<<<<<< HEAD
    void refresh(); //copy backbuffer to actual displaybuffer so all new drawn changes are visible at the same moment.
=======
>>>>>>> include_gpu
    void clear(color back_ground);
    color get_pixel(vector_2_u32 position);
    void put_transparent_pixel(vector_2_u32 position, color own_color);
    void put_opaque_pixel(vector_2_u32 position, color own_color);
    void draw_line(vector_2_u32 begin, vector_2_u32 end, color own_color);
    void draw_text(const char* text, vector_2_u32 position);
};

inline color screen::get_pixel(vector_2_u32 position)
{
<<<<<<< HEAD
    return backbuffer[position.coordinate[0] * footprint.size.coordinate[0] + position.coordinate[0]];
=======
    return displaybuffer[position.coordinate[0] * footprint.size.coordinate[0] + position.coordinate[0]];
>>>>>>> include_gpu
}

inline void screen::put_transparent_pixel(vector_2_u32 position, color own_color)
{
    u32 alpha = own_color.alpha;
    u32 anti_alpha = 255 - alpha;

<<<<<<< HEAD
    color a_color = backbuffer[position.coordinate[1] * footprint.size.coordinate[0] + position.coordinate[0]];

=======
    color a_color = displaybuffer[position.coordinate[1] * footprint.size.coordinate[0] + position.coordinate[0]];

    //to be completely correct we should divide by 255 and not >> 8 (which is divide by 256), but we do do it because it is much faster.
>>>>>>> include_gpu
    a_color.red = static_cast<u8>((a_color.red * anti_alpha + own_color.red * alpha) >> 8);
    a_color.green = static_cast<u8>((a_color.green * anti_alpha + own_color.green * alpha) >> 8);
    a_color.blue = static_cast<u8>((a_color.blue * anti_alpha + own_color.blue * alpha) >> 8);

<<<<<<< HEAD
    backbuffer[position.coordinate[1] * footprint.size.coordinate[0] + position.coordinate[0]] = a_color;
=======
    displaybuffer[position.coordinate[1] * footprint.size.coordinate[0] + position.coordinate[0]] = a_color;
>>>>>>> include_gpu
}

inline void screen::put_opaque_pixel(vector_2_u32 position, color own_color)
{
<<<<<<< HEAD
    backbuffer[position.coordinate[1] * footprint.size.coordinate[0] + position.coordinate[0]] = own_color;
=======
    displaybuffer[position.coordinate[1] * footprint.size.coordinate[0] + position.coordinate[0]] = own_color;
>>>>>>> include_gpu
}
#include "screen.h"
#include "font.h"

screen::screen(vector_2_u32 size, color* buffer) :
    footprint(vector_2_u32(0, 0), size),
    displaybuffer(buffer)
{
}

screen::~screen()
{
}

rectangle* screen::get_footprint()
<<<<<<< HEAD
{
    return &footprint;
}

void screen::refresh()
=======
>>>>>>> include_gpu
{
    u64* source = reinterpret_cast<u64*>(backbuffer);
    u64* destination = reinterpret_cast<u64*>(displaybuffer);
    for(u32 i = 0; i < 800 * 480 / 2; i++)
    {
        *destination = *source;
        source++;
        destination++;
    }
}

void screen::clear(color background)
{
    for(u32 x = 0; x < footprint.size.coordinate[0]; x++)
        for(u32 y = 0; y < footprint.size.coordinate[1]; y++)
            put_opaque_pixel(vector_2_u32(x, y), background);
}

//functions used in draw_line
static inline u32 ipart(f32 x)
{
    return static_cast<u32>(x);
}

static inline f32 rnd(f32 x)
{
    return ipart(x + 0.5);
}

static inline f32 flr(f32 x)
{
    if(x > 0)
        return static_cast<u32>(x);
    return static_cast<u32>(x - 1.0f);
}

static inline f32 fpart(f32 x)
{
    if(x < 0.0f)
        return 1.0f - (x - flr(x));
    return x - flr(x);
}

static inline f32 rfpart(f32 x)
{
    return 1.0f - fpart(x);
}

static inline u32 abs_i(u32 x)
{
    return (x > 0) ? x : -x;
}

void screen::draw_line(vector_2_u32 begin, vector_2_u32 end, color own_color)
{
    color modified_color(own_color);

    bool steep = abs_i(end.coordinate[1] - begin.coordinate[1]) > abs_i(end.coordinate[0] - begin.coordinate[0]);
    if(steep)
    {
        f32 swap;

        swap = begin.coordinate[0];
        begin.coordinate[0] = begin.coordinate[1];
        begin.coordinate[1] = swap;

        swap = end.coordinate[0];
        end.coordinate[0] = end.coordinate[1];
        end.coordinate[1] = swap;
    }

    if(begin.coordinate[0] > end.coordinate[0])
    {
        f32 swap;

        swap = begin.coordinate[0];
        begin.coordinate[0] = end.coordinate[0];
        end.coordinate[0] = swap;

        swap = begin.coordinate[1];
        begin.coordinate[1] = end.coordinate[1];
        end.coordinate[1] = swap;
    }

    f32 d_x = (f32)(end.coordinate[0] - begin.coordinate[0]);
    f32 d_y = (f32)(end.coordinate[1] - begin.coordinate[1]);
    f32 gradient = d_y / d_x;

    // handle first endpoint
    f32 x_end = rnd((f32)begin.coordinate[0]);
    f32 y_end = begin.coordinate[1] + gradient * (x_end - begin.coordinate[0]);
    f32 xgap = rfpart(begin.coordinate[0] + 0.5f);
    u32 x_pxl_begin = x_end; // this will be used in the main loop
    u32 y_pxl_begin = ipart(y_end);
    if(steep)
    {
        modified_color.alpha = static_cast<u32>(own_color.alpha * rfpart(y_end) * xgap);
        put_transparent_pixel(vector_2_u32(y_pxl_begin, x_pxl_begin), modified_color);
        modified_color.alpha = static_cast<u32>(own_color.alpha * fpart(y_end) * xgap);
        put_transparent_pixel(vector_2_u32(y_pxl_begin + 1, x_pxl_begin), modified_color);
    }
    else
    {
        modified_color.alpha = static_cast<u32>(own_color.alpha * rfpart(y_end) * xgap);
        put_transparent_pixel(vector_2_u32(x_pxl_begin, y_pxl_begin), modified_color);
        modified_color.alpha = static_cast<u32>(own_color.alpha * fpart(y_end) * xgap);
        put_transparent_pixel(vector_2_u32(x_pxl_begin + 1, y_pxl_begin), modified_color);
    }
    f32 inter_y = y_end + gradient; // first y-intersection for the main loop

    // handle second endpoint
    x_end = rnd((f32)end.coordinate[0]);
    y_end = end.coordinate[1] + gradient * (x_end - end.coordinate[0]);
    xgap = fpart(end.coordinate[0] + 0.5f);
    u32 x_pxl_end = static_cast<u32>(x_end); //this will be used in the main loop
    u32 y_pxl_end = ipart(y_end);
    if(steep)
    {
        modified_color.alpha = static_cast<u32>(own_color.alpha * rfpart(y_end) * xgap);
        put_transparent_pixel(vector_2_u32(y_pxl_end, x_pxl_end), modified_color);
        modified_color.alpha = static_cast<u32>(own_color.alpha * fpart(y_end) * xgap);
        put_transparent_pixel(vector_2_u32(y_pxl_end + 1, x_pxl_end), modified_color);
    }
    else
    {
        modified_color.alpha = static_cast<u32>(own_color.alpha * rfpart(y_end) * xgap);
        put_transparent_pixel(vector_2_u32(x_pxl_end, y_pxl_end), modified_color);
        modified_color.alpha = static_cast<u32>(own_color.alpha * fpart(y_end) * xgap);
        put_transparent_pixel(vector_2_u32(x_pxl_end + 1, y_pxl_end), modified_color);
    }

    // main loop
    if(steep)
    {
        for(u32 x = x_pxl_begin + 1; x < x_pxl_end; x++)
        {
            modified_color.alpha = static_cast<u32>(own_color.alpha * rfpart(inter_y));
            put_transparent_pixel(vector_2_u32(ipart(inter_y), x), modified_color);
            modified_color.alpha = static_cast<u32>(own_color.alpha * fpart(inter_y));
            put_transparent_pixel(vector_2_u32(ipart(inter_y) + 1, x), modified_color);
            inter_y += gradient;
        }
    }
    else
    {
        for(u32 x = x_pxl_begin + 1; x < x_pxl_end; x++)
        {
            modified_color.alpha = static_cast<u32>(own_color.alpha * rfpart(inter_y));
            put_transparent_pixel(vector_2_u32(x, ipart(inter_y)), modified_color);
            modified_color.alpha = static_cast<u32>(own_color.alpha * fpart(inter_y));
            put_transparent_pixel(vector_2_u32(x, ipart(inter_y) + 1), modified_color);
            inter_y += gradient;
        }
    }
}

void screen::draw_text(const char* text, vector_2_u32 position)
{
    u32 original_x = position.coordinate[0];
    while(*text != '\0')
    {
        if(*text == '\n' || *text == '\r')
        {
            position.coordinate[0] = original_x;
<<<<<<< HEAD
            position.coordinate[1] += font::letter_height + 1;
=======
            position.coordinate[1] += font::letter_height;
>>>>>>> include_gpu
        }
        else
        {
            for(u32 x = 0; x < font::letter_width; x++)
                for(u32 y = 0; y < font::letter_height; y++)
                {
                    u8 greyscale = font::monospace[static_cast<u32>(*text)][y][x];
<<<<<<< HEAD
                    put_transparent_pixel(vector_2_u32::sum(position, vector_2_u32(x, y)), color(0,0,0,greyscale));
                }
            position.coordinate[0] += font::letter_width + 1;
=======
                    put_transparent_pixel(vector_2_u32::sum(position, vector_2_u32(x, y)), color(0,100,0,greyscale));
                }
            position.coordinate[0] += font::letter_width;
>>>>>>> include_gpu
        }
        text++;
    }
}

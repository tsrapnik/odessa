#include "screen.h"

screen::screen(u32 x_size, u32 y_size) :
    footprint(vector_2_int(0, 0), vector_2_int(x_size, y_size))
{
}

screen::~screen()
{
}

char* screen::get_buffer()
{
    return (char*)buffer;
}

rectangle* screen::get_footprint()
{
    return &footprint;
}

void screen::clear(colour back_ground)
{
    for(int x = 0; x < footprint.size.x; x++)
        for(int y = 0; y < footprint.size.y; y++)
            put_opaque_pixel(vector_2_int(x, y), back_ground);
}

//functions used in draw_line
static inline int ipart(float x)
{
    return (int)x;
}

static inline float rnd(float x)
{
    return ipart(x + 0.5);
}

static inline float flr(float x)
{
    if(x > 0)
        return (int)x;
    return (int)(x - 1.0f);
}

static inline float fpart(float x)
{
    if(x < 0.0f)
        return 1.0f - (x - flr(x));
    return x - flr(x);
}

static inline float rfpart(float x)
{
    return 1.0f - fpart(x);
}

static inline int abs_i(int x)
{
    return (x > 0) ? x : -x;
}

void screen::draw_line(vector_2_int begin, vector_2_int end, colour own_colour)
{
    colour modified_colour(own_colour);

    bool steep = abs_i(end.y - begin.y) > abs_i(end.x - begin.x);
    if(steep)
    {
        float swap;

        swap = begin.x;
        begin.x = begin.y;
        begin.y = swap;

        swap = end.x;
        end.x = end.y;
        end.y = swap;
    }

    if(begin.x > end.x)
    {
        float swap;

        swap = begin.x;
        begin.x = end.x;
        end.x = swap;

        swap = begin.y;
        begin.y = end.y;
        end.y = swap;
    }

    float d_x = (float)(end.x - begin.x);
    float d_y = (float)(end.y - begin.y);
    float gradient = d_y / d_x;

    // handle first endpoint
    float x_end = rnd((float)begin.x);
    float y_end = begin.y + gradient * (x_end - begin.x);
    float xgap = rfpart(begin.x + 0.5f);
    int x_pxl_begin = x_end; // this will be used in the main loop
    int y_pxl_begin = ipart(y_end);
    if(steep)
    {
        modified_colour.alpha = (int)(own_colour.alpha * rfpart(y_end) * xgap);
        put_transparent_pixel(vector_2_int(y_pxl_begin, x_pxl_begin), modified_colour);
        modified_colour.alpha = (int)(own_colour.alpha * fpart(y_end) * xgap);
        put_transparent_pixel(vector_2_int(y_pxl_begin + 1, x_pxl_begin), modified_colour);
    }
    else
    {
        modified_colour.alpha = (int)(own_colour.alpha * rfpart(y_end) * xgap);
        put_transparent_pixel(vector_2_int(x_pxl_begin, y_pxl_begin), modified_colour);
        modified_colour.alpha = (int)(own_colour.alpha * fpart(y_end) * xgap);
        put_transparent_pixel(vector_2_int(x_pxl_begin + 1, y_pxl_begin), modified_colour);
    }
    float inter_y = y_end + gradient; // first y-intersection for the main loop

    // handle second endpoint
    x_end = rnd((float)end.x);
    y_end = end.y + gradient * (x_end - end.x);
    xgap = fpart(end.x + 0.5f);
    int x_pxl_end = (int)x_end; //this will be used in the main loop
    int y_pxl_end = ipart(y_end);
    if(steep)
    {
        modified_colour.alpha = (int)(own_colour.alpha * rfpart(y_end) * xgap);
        put_transparent_pixel(vector_2_int(y_pxl_end, x_pxl_end), modified_colour);
        modified_colour.alpha = (int)(own_colour.alpha * fpart(y_end) * xgap);
        put_transparent_pixel(vector_2_int(y_pxl_end + 1, x_pxl_end), modified_colour);
    }
    else
    {
        modified_colour.alpha = (int)(own_colour.alpha * rfpart(y_end) * xgap);
        put_transparent_pixel(vector_2_int(x_pxl_end, y_pxl_end), modified_colour);
        modified_colour.alpha = (int)(own_colour.alpha * fpart(y_end) * xgap);
        put_transparent_pixel(vector_2_int(x_pxl_end + 1, y_pxl_end), modified_colour);
    }

    // main loop
    if(steep)
    {
        for(int x = x_pxl_begin + 1; x < x_pxl_end; x++)
        {
            modified_colour.alpha = (int)(own_colour.alpha * rfpart(inter_y));
            put_transparent_pixel(vector_2_int(ipart(inter_y), x), modified_colour);
            modified_colour.alpha = (int)(own_colour.alpha * fpart(inter_y));
            put_transparent_pixel(vector_2_int(ipart(inter_y) + 1, x), modified_colour);
            inter_y += gradient;
        }
    }
    else
    {
        for(int x = x_pxl_begin + 1; x < x_pxl_end; x++)
        {
            modified_colour.alpha = (int)(own_colour.alpha * rfpart(inter_y));
            put_transparent_pixel(vector_2_int(x, ipart(inter_y)), modified_colour);
            modified_colour.alpha = (int)(own_colour.alpha * fpart(inter_y));
            put_transparent_pixel(vector_2_int(x, ipart(inter_y) + 1), modified_colour);
            inter_y += gradient;
        }
    }
}

void screen::draw_text(const char* text, vector_2_int position)
{
    int original_x = position.x;
    const colour* font = (const colour*)0x4b019000;
    while(*text)
    {
        if(*text == '\n' || *text == '\r')
        {
            position.x = original_x;
            position.y += 21;
        }
        else
        {
            for(int x = 0; x < 10; x++)
                for(int y = 0; y < 21; y++)
                    put_transparent_pixel(vector_2_int::add(position, vector_2_int(x, y)), font[*text * 10 * 21 + x * 21 + y]);
            position.x += 10;
        }
        text++;
    }
}

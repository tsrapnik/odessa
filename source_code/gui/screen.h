#pragma once

#include "type_definitions.h"
#include "colour.h"
#include "rectangle.h"

class screen
{
private:
	//tmp
	//only 32 bit colours supported.
	//this struct is shared between g_p_u and a_r_m side
	// must be 16 byte aligned in memory
	struct bcm2835_frame_buffer_info
	{
		u32 width;		// physical width of display in pixel
		u32 height;		// physical height of display in pixel
		u32 virt_width;		// always as physical width so far
		u32 virt_height;		// always as physical height so far
		u32 pitch;		// should be init with 0
		u32 depth;		// number of bits per pixel
		u32 offset_x;		// normally zero
		u32 offset_y;		// normally zero
		u32 buffer_ptr;		// address of frame buffer (init with 0, set by g_p_u)
		u32 buffer_size;		// size of frame buffer (init with 0, set by g_p_u)
	}
	__attribute__ ((packed));

	rectangle footprint;
	colour* buffer;
public:
	screen( u32 x_size, u32 y_size );
	~screen();
	char* get_buffer();
	rectangle* get_footprint();
	void clear( colour back_ground );
	colour get_pixel( vector2_i position );
	void put_transparent_pixel( vector2_i position, colour own_colour );
	void put_opaque_pixel( vector2_i position, colour own_colour );
	void draw_line( vector2_i begin, vector2_i end, colour own_colour );
	void draw_text( const char* text, vector2_i position );
};

inline colour screen::get_pixel( vector2_i position )
{
	//use some syntactic sugar trick to hide the original buffer pointer with a multidimensional
	//one, so you can acces it in the buffer[i][j] way in stead of using buffer[i*width+j].
	colour ( * buffer )[ footprint.size.x ] = ( colour ( * )[ footprint.size.x ] )this->buffer;
	
	return buffer[ position.y ][ position.x ];
}

inline void screen::put_transparent_pixel( vector2_i position, colour own_colour )
{
	//use some syntactic sugar trick to hide the original buffer pointer with a multidimensional
	//one, so you can acces it in the buffer[i][j] way in stead of using buffer[i*width+j].
	colour ( * buffer )[ footprint.size.x ] = ( colour ( * )[ footprint.size.x ] )this->buffer;

	int alpha = own_colour.alpha;
	int anti_alpha = 255 - alpha;

	colour this_colour = buffer[ position.y ][ position.x ];

	this_colour.red = ( unsigned char )( ( this_colour.red * anti_alpha + own_colour.red * alpha ) >> 8 );
	this_colour.green = ( unsigned char )( ( this_colour.green * anti_alpha + own_colour.green * alpha ) >> 8 );
	this_colour.blue = ( unsigned char )( ( this_colour.blue * anti_alpha + own_colour.blue * alpha ) >> 8 );

	buffer[ position.y ][ position.x ] = this_colour;
}

inline void screen::put_opaque_pixel( vector2_i position, colour own_colour )
{
	//use some syntactic sugar trick to hide the original buffer pointer with a multidimensional
	//one, so you can acces it in the buffer[i][j] way in stead of using buffer[i*width+j].
	colour ( * buffer )[ footprint.size.x ] = ( colour ( * )[ footprint.size.x ] )this->buffer;

	buffer[ position.y ][ position.x ] = own_colour;
}
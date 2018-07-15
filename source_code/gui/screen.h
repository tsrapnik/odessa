#pragma once

#include "typeDefinitions.h"
#include "colour.h"
#include "rectangle.h"

class screen
{
private:
	//tmp
	//Only 32 bit colours supported.
	//This struct is shared between GPU and ARM side
	// Must be 16 byte aligned in memory
	struct Bcm2835FrameBufferInfo
	{
		u32 Width;		// Physical width of display in pixel
		u32 Height;		// Physical height of display in pixel
		u32 VirtWidth;		// always as physical width so far
		u32 VirtHeight;		// always as physical height so far
		u32 Pitch;		// Should be init with 0
		u32 Depth;		// Number of bits per pixel
		u32 OffsetX;		// Normally zero
		u32 OffsetY;		// Normally zero
		u32 BufferPtr;		// Address of frame buffer (init with 0, set by GPU)
		u32 BufferSize;		// Size of frame buffer (init with 0, set by GPU)
	}
	__attribute__ ((packed));

	rectangle footprint;
	colour* buffer;
public:
	screen( u32 x_size, u32 y_size );
	~screen();
	char* getBuffer();
	rectangle* getFootprint();
	void clear( colour backGround );
	colour getPixel( vector2I position );
	void putTransparentPixel( vector2I position, colour ownColour );
	void putOpaquePixel( vector2I position, colour ownColour );
	void drawLine( vector2I begin, vector2I end, colour ownColour );
	void drawText( const char* text, vector2I position );
};

inline colour screen::getPixel( vector2I position )
{
	//Use some syntactic sugar trick to hide the original buffer pointer with a multidimensional
	//one, so you can acces it in the buffer[i][j] way in stead of using buffer[i*width+j].
	colour ( * buffer )[ footprint.size.x ] = ( colour ( * )[ footprint.size.x ] )this->buffer;
	
	return buffer[ position.y ][ position.x ];
}

inline void screen::putTransparentPixel( vector2I position, colour ownColour )
{
	//Use some syntactic sugar trick to hide the original buffer pointer with a multidimensional
	//one, so you can acces it in the buffer[i][j] way in stead of using buffer[i*width+j].
	colour ( * buffer )[ footprint.size.x ] = ( colour ( * )[ footprint.size.x ] )this->buffer;

	int alpha = ownColour.alpha;
	int antiAlpha = 255 - alpha;

	colour thisColour = buffer[ position.y ][ position.x ];

	thisColour.red = ( unsigned char )( ( thisColour.red * antiAlpha + ownColour.red * alpha ) >> 8 );
	thisColour.green = ( unsigned char )( ( thisColour.green * antiAlpha + ownColour.green * alpha ) >> 8 );
	thisColour.blue = ( unsigned char )( ( thisColour.blue * antiAlpha + ownColour.blue * alpha ) >> 8 );

	buffer[ position.y ][ position.x ] = thisColour;
}

inline void screen::putOpaquePixel( vector2I position, colour ownColour )
{
	//Use some syntactic sugar trick to hide the original buffer pointer with a multidimensional
	//one, so you can acces it in the buffer[i][j] way in stead of using buffer[i*width+j].
	colour ( * buffer )[ footprint.size.x ] = ( colour ( * )[ footprint.size.x ] )this->buffer;

	buffer[ position.y ][ position.x ] = ownColour;
}
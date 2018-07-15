#include "screen.h"

screen::screen( u32 x_size, u32 y_size ):
	footprint( vector2I( 0, 0 ), vector2I( x_size, y_size ) )
{

}

screen::~screen()
{}

char* screen::getBuffer()
{
	return ( char* )buffer;
}

rectangle* screen::getFootprint()
{
	return &footprint;
}

void screen::clear( colour backGround )
{
	for( int x = 0; x < footprint.size.x; x++ )
		for( int y = 0; y < footprint.size.y; y++ )
			putOpaquePixel( vector2I( x, y ), backGround );
}

//Functions used in drawLine
static inline int ipart( float x )
{
	return ( int )x;
}

static inline float rnd( float x )
{
	return ipart( x + 0.5 );
}

static inline float flr( float x )
{
	if( x > 0 )
		return ( int )x;
	return ( int )( x - 1.0f );
}

static inline float fpart( float x )
{
	if( x < 0.0f )
		return 1.0f - ( x - flr( x ) );
	return	x - flr( x );
}

static inline float rfpart( float x )
{
	return 1.0f - fpart( x );
}

static inline int absI( int x )
{
	return ( x > 0 ) ? x : -x;
}

void screen::drawLine( vector2I begin, vector2I end, colour ownColour )
{
	colour modifiedColour( ownColour );

	bool steep = absI( end.y - begin.y ) > absI( end.x - begin.x );
	if( steep )
	{
		float swap;

		swap = begin.x;
		begin.x = begin.y;
		begin.y = swap;

		swap = end.x;
		end.x = end.y;
		end.y = swap;
	}

	if( begin.x > end.x )
	{
		float swap;

		swap = begin.x;
		begin.x = end.x;
		end.x = swap;

		swap = begin.y;
		begin.y = end.y;
		end.y = swap;
	}

	float dX = ( float )( end.x - begin.x );
	float dY = ( float )( end.y - begin.y );
	float gradient = dY / dX;

	// handle first endpoint
	float xEnd = rnd( ( float )begin.x );
	float yEnd = begin.y + gradient * ( xEnd - begin.x );
	float xgap = rfpart( begin.x + 0.5f );
	int xPxlBegin = xEnd; // this will be used in the main loop
	int yPxlBegin = ipart( yEnd );
	if( steep )
	{
		modifiedColour.alpha = ( int )( ownColour.alpha * rfpart( yEnd ) * xgap );
		putTransparentPixel( vector2I( yPxlBegin, xPxlBegin ), modifiedColour );
		modifiedColour.alpha = ( int )( ownColour.alpha * fpart( yEnd ) * xgap );
		putTransparentPixel( vector2I( yPxlBegin + 1, xPxlBegin ), modifiedColour );
	}
	else
	{
		modifiedColour.alpha = ( int )( ownColour.alpha * rfpart( yEnd ) * xgap );
		putTransparentPixel( vector2I( xPxlBegin, yPxlBegin ), modifiedColour );
		modifiedColour.alpha = ( int )( ownColour.alpha * fpart( yEnd ) * xgap );
		putTransparentPixel( vector2I( xPxlBegin + 1, yPxlBegin ), modifiedColour );
	}
	float interY = yEnd + gradient; // first y-intersection for the main loop

	// handle second endpoint
	xEnd = rnd( ( float )end.x );
	yEnd = end.y + gradient * ( xEnd - end.x );
	xgap = fpart( end.x + 0.5f );
	int xPxlEnd = ( int ) xEnd; //this will be used in the main loop
	int yPxlEnd = ipart( yEnd );
	if( steep )
	{
		modifiedColour.alpha = ( int )( ownColour.alpha * rfpart( yEnd ) * xgap );
		putTransparentPixel( vector2I( yPxlEnd, xPxlEnd ), modifiedColour );
		modifiedColour.alpha = ( int )( ownColour.alpha * fpart( yEnd ) * xgap );
		putTransparentPixel( vector2I( yPxlEnd + 1, xPxlEnd ), modifiedColour );
	}
	else
	{
		modifiedColour.alpha = ( int )( ownColour.alpha * rfpart( yEnd ) * xgap );
		putTransparentPixel( vector2I( xPxlEnd, yPxlEnd ), modifiedColour );
		modifiedColour.alpha = ( int )( ownColour.alpha * fpart( yEnd ) * xgap );
		putTransparentPixel( vector2I( xPxlEnd + 1, yPxlEnd ), modifiedColour );
	}

	// main loop
	if( steep )
	{
		for( int x = xPxlBegin + 1; x < xPxlEnd; x++ )
		{
			modifiedColour.alpha = ( int )( ownColour.alpha * rfpart( interY ) );
			putTransparentPixel( vector2I( ipart( interY ), x ), modifiedColour );
			modifiedColour.alpha = ( int )( ownColour.alpha * fpart( interY ) );
			putTransparentPixel( vector2I( ipart( interY ) + 1, x ), modifiedColour );
			interY += gradient;
		}
	}
	else
	{
		for( int x = xPxlBegin + 1; x < xPxlEnd; x++ )
		{
			modifiedColour.alpha = ( int )( ownColour.alpha * rfpart( interY ) );
			putTransparentPixel( vector2I( x, ipart( interY ) ), modifiedColour );
			modifiedColour.alpha = ( int )( ownColour.alpha * fpart( interY ) );
			putTransparentPixel( vector2I( x, ipart( interY ) + 1 ), modifiedColour );
			interY += gradient;
		}
	}
}

void screen::drawText( const char* text, vector2I position )
{
	int originalX = position.x;
	const colour* font = ( const colour* )0x4B019000;
	while( *text )
	{
		if( *text == '\n' || *text == '\r' )
		{
			position.x = originalX;
			position.y += 21;
		}
		else
		{
			for( int x = 0; x < 10; x++ )
				for( int y = 0; y < 21; y++ )
					putTransparentPixel( vector2I::add( position, vector2I( x, y ) ), font[ *text * 10 * 21 + x * 21 + y ] );
			position.x += 10;
		}
		text++;
	}
}


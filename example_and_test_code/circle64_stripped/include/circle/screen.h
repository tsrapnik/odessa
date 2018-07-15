#ifndef _circle_screen_h
#define _circle_screen_h

#include <circle/bcmframebuffer.h>

//#define DEPTH	32		// can be: 8, 16 or 32
/*
// really ((green) & 0x3F) << 5, but to have a 0-31 range for all colors
#define COLOR16(red, green, blue)	  (((red) & 0x1F) << 11 \
					| ((green) & 0x1F) << 6 \
					| ((blue) & 0x1F))

#define COLOR32(red, green, blue, alpha)  (((red) & 0xFF)        \
					| ((green) & 0xFF) << 8  \
					| ((blue) & 0xFF) << 16  \
					| ((alpha) & 0xFF) << 24)

#define BLACK_COLOR	0
*/

typedef u32 TScreenColor;
/*
#define NORMAL_COLOR			COLOR32 (255, 255, 255, 255)
#define HIGH_COLOR			COLOR32 (255, 0, 0, 255)
#define HALF_COLOR			COLOR32 (0, 0, 255, 255)

struct TScreenStatus
{
	TScreenColor   *pContent;
	unsigned	nState;
	unsigned	nCursorX;
	unsigned	nCursorY;
	boolean		bCursorOn;
	TScreenColor	Color;
	boolean		bInsertOn;
	unsigned	nParam1;
	unsigned	nParam2;
	boolean		bUpdated;
};*/

class CScreenDevice
{
public:
	CScreenDevice (unsigned nWidth, unsigned nHeight, boolean bVirtual = FALSE);

public:
	CBcmFrameBuffer	*m_pFrameBuffer;
	TScreenColor  	*m_pBuffer;
};

#endif

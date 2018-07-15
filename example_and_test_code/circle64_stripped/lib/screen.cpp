#include <circle/screen.h>

CScreenDevice::CScreenDevice (unsigned nWidth, unsigned nHeight, boolean bVirtual)
{
	m_pFrameBuffer = new CBcmFrameBuffer (nWidth, nHeight, 32);
	m_pFrameBuffer->Initialize ();
	m_pBuffer = (TScreenColor *) (u64) m_pFrameBuffer->GetBuffer ();
}
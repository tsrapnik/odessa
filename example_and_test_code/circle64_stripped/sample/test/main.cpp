#include <circle/alloc.h>
#include <circle/translationtable.h>
#include <circle/bcmframebuffer.h>

CTranslationTable *m_pTranslationTable;
u64 m_nMemSize;

#define ATTRINDX_NORMAL		0
#define ATTRINDX_DEVICE		1
#define ATTRINDX_COHERENT	2

#define SCTLR_EL1_WXN		(1 << 19)		// SCTLR_EL1
#define SCTLR_EL1_I		(1 << 12)
#define SCTLR_EL1_C		(1 << 2)
#define SCTLR_EL1_A		(1 << 1)
#define SCTLR_EL1_M		(1 << 0)

#define TCR_EL1_IPS__SHIFT	32			// TCR_EL1
#define TCR_EL1_IPS__MASK	(7UL << 32)
	#define TCR_EL1_IPS_4GB		0UL
#define TCR_EL1_EPD1		(1 << 23)
#define TCR_EL1_A1		(1 << 22)
#define TCR_EL1_TG0__SHIFT	14
#define TCR_EL1_TG0__MASK	(3 << 14)
	#define TCR_EL1_TG0_64KB	1
#define TCR_EL1_SH0__SHIFT	12
#define TCR_EL1_SH0__MASK	(3 << 12)
	#define TCR_EL1_SH0_INNER	3
#define TCR_EL1_ORGN0__SHIFT	10
#define TCR_EL1_ORGN0__MASK	(3 << 10)
	#define TCR_EL1_ORGN0_WR_BACK	3
#define TCR_EL1_IRGN0__SHIFT	8
#define TCR_EL1_IRGN0__MASK	(3 << 8)
	#define TCR_EL1_IRGN0_WR_BACK	3
#define TCR_EL1_EPD0		(1 << 7)
#define TCR_EL1_T0SZ__SHIFT	0
#define TCR_EL1_T0SZ__MASK	(0x3F << 0)
	#define TCR_EL1_T0SZ_4GB	32


void enable_mmu()
{
	u64 nMAIR_EL1 =   0xCC << ATTRINDX_NORMAL*8	// inner/outer write-back non-transient, non-allocating
	                | 0x04 << ATTRINDX_DEVICE*8	// Device-nGnRE
	                | 0x00 << ATTRINDX_COHERENT*8;	// Device-nGnRnE
	asm volatile ("msr mair_el1, %0" : : "r" (nMAIR_EL1));

	asm volatile ("msr ttbr0_el1, %0" : : "r" (m_pTranslationTable->GetBaseAddress ()));

	u64 nTCR_EL1;
	asm volatile ("mrs %0, tcr_el1" : "=r" (nTCR_EL1));
	nTCR_EL1 &= ~(  TCR_EL1_IPS__MASK
		      | TCR_EL1_A1
		      | TCR_EL1_TG0__MASK
		      | TCR_EL1_SH0__MASK
		      | TCR_EL1_ORGN0__MASK
		      | TCR_EL1_IRGN0__MASK
		      | TCR_EL1_EPD0
		      | TCR_EL1_T0SZ__MASK);
	nTCR_EL1 |=   TCR_EL1_EPD1
		    | TCR_EL1_IPS_4GB	    << TCR_EL1_IPS__SHIFT
		    | TCR_EL1_TG0_64KB	    << TCR_EL1_TG0__SHIFT
		    | TCR_EL1_SH0_INNER     << TCR_EL1_SH0__SHIFT
		    | TCR_EL1_ORGN0_WR_BACK << TCR_EL1_ORGN0__SHIFT
		    | TCR_EL1_IRGN0_WR_BACK << TCR_EL1_IRGN0__SHIFT
		    | TCR_EL1_T0SZ_4GB	    << TCR_EL1_T0SZ__SHIFT;
	asm volatile ("msr tcr_el1, %0" : : "r" (nTCR_EL1));

	u32 nSCTLR_EL1;
	asm volatile ("mrs %0, sctlr_el1" : "=r" (nSCTLR_EL1));
	nSCTLR_EL1 &= ~(  SCTLR_EL1_WXN
			| SCTLR_EL1_A);
	nSCTLR_EL1 |=   SCTLR_EL1_I
		      | SCTLR_EL1_C
		      | SCTLR_EL1_M;
	asm volatile ("msr sctlr_el1, %0" : : "r" (nSCTLR_EL1) : "memory");
}

void initialize_memory()
{
	mem_init ( 0, ( 512 - 64 ) * 0x100000 );

	m_pTranslationTable = new CTranslationTable (m_nMemSize);
	enable_mmu();
}

int main (void)
{
	initialize_memory();

	CBcmMailBox m_MailBox(MAILBOX_CHANNEL_FB);
	volatile Bcm2835FrameBufferInfo* m_pInfo = new Bcm2835FrameBufferInfo;
	m_pInfo->Width      = 800;
	m_pInfo->Height     = 480;
	m_pInfo->VirtWidth  = 800;
	m_pInfo->VirtHeight = 480;
	m_pInfo->Pitch      = 0;
	m_pInfo->Depth      = 32;
	m_pInfo->OffsetX    = 0;
	m_pInfo->OffsetY    = 0;
	m_pInfo->BufferPtr  = 0;
	m_pInfo->BufferSize = 0;

	CleanDataCache ();
	DataSyncBarrier ();
	u32 nResult = m_MailBox.WriteRead (0xC0000000 + (u32) (u64) m_pInfo);
	InvalidateDataCache ();
	DataMemBarrier ();

	u32* buffer = (u32 *) (u64)( m_pInfo->BufferPtr & 0x3FFFFFFF );
	
	int color = 0x00ff0000;
	while( true )
	{
		for( u32 x = 0; x < 800; x++ )
			for( u32 y = 0; y < 480; y ++ )
				buffer[ 800 * y + x ] = color;
		color++;
	}
}

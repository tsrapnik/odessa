//#include <circle/alloc.h>
//#include <circle/translationtable.h>
//#include <circle/armv8mmu.h>

typedef unsigned int		u32;
typedef unsigned long		u64;
typedef unsigned long	size_t;

typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned long		u64;

#define ARMV8MMU_TABLE_ENTRIES		8192

struct TARMV8MMU_LEVEL2_TABLE_DESCRIPTOR
{
	u64	Value11		: 2,		// set to 3
		Ignored1	: 14,		// set to 0
		TableAddress	: 32,		// table base address [47:16]
		Reserved0	: 4,		// set to 0
		Ignored2	: 7,		// set to 0
		PXNTable	: 1,		// set to 0
		UXNTable	: 1,		// set to 0
		APTable		: 2,
#define AP_TABLE_ALL_ACCESS		0
		NSTable		: 1;		// RES0, set to 0
}
__attribute__ ((packed));

#define ARMV8MMUL2TABLEADDR(addr)	(((addr) >> 16) & 0xFFFFFFFF)
#define ARMV8MMUL2TABLEPTR(table)	((void *) ((table) << 16))

struct TARMV8MMU_LEVEL2_BLOCK_DESCRIPTOR
{
	u64	Value01		: 2,		// set to 1
		//LowerAttributes	: 10,
			AttrIndx	: 3,	// [2:0], see MAIR_EL1
			NS		: 1,	// RES0, set to 0
			AP		: 2,	// [2:1]
#define ATTRIB_AP_RW_EL1		0
#define ATTRIB_AP_RW_ALL		1
#define ATTRIB_AP_RO_EL1		2
#define ATTRIB_AP_RO_ALL		3
			SH		: 2,	// [1:0]
#define ATTRIB_SH_NON_SHAREABLE		0
#define ATTRIB_SH_OUTER_SHAREABLE	2
#define ATTRIB_SH_INNER_SHAREABLE	3
			AF		: 1,	// set to 1, will fault otherwise
			nG		: 1,	// set to 0
		Reserved0_1	: 17,		// set to 0
		OutputAddress	: 19,		// [47:29]
		Reserved0_2	: 4,		// set to 0
		//UpperAttributes	: 12
			Continous	: 1,	// set to 0
			PXN		: 1,	// set to 0, 1 for device memory
			UXN		: 1,	// set to 1
			Ignored		: 9;	// set to 0
}
__attribute__ ((packed));

#define ARMV8MMU_LEVEL2_BLOCK_SIZE	(512 * MEGABYTE)
#define ARMV8MMUL2BLOCKADDR(addr)	(((addr) >> 29) & 0x7FFFF)
#define ARMV8MMUL2BLOCKPTR(block)	((void *) ((table) << 29))

struct TARMV8MMU_LEVEL2_INVALID_DESCRIPTOR
{
	u64	Value0		: 1,		// set to 0
		Ignored		: 63;
}
__attribute__ ((packed));

union TARMV8MMU_LEVEL2_DESCRIPTOR
{
	TARMV8MMU_LEVEL2_TABLE_DESCRIPTOR	Table;
	TARMV8MMU_LEVEL2_BLOCK_DESCRIPTOR	Block;
	TARMV8MMU_LEVEL2_INVALID_DESCRIPTOR	Invalid;
}
__attribute__ ((packed));

struct TARMV8MMU_LEVEL3_PAGE_DESCRIPTOR
{
	u64	Value11		: 2,		// set to 3
		//LowerAttributes	: 10,
			AttrIndx	: 3,	// [2:0], see MAIR_EL1
			NS		: 1,	// RES0, set to 0
			AP		: 2,	// [2:1]
			SH		: 2,	// [1:0]
			AF		: 1,	// set to 1, will fault otherwise
			nG		: 1,	// set to 0
		Reserved0_1	: 4,		// set to 0
		OutputAddress	: 32,		// [47:16]
		Reserved0_2	: 4,		// set to 0
		//UpperAttributes	: 12
			Continous	: 1,	// set to 0
			PXN		: 1,	// set to 0, 1 for device memory
			UXN		: 1,	// set to 1
			Ignored		: 9	// set to 0
		;
}
__attribute__ ((packed));

#define ARMV8MMU_LEVEL3_PAGE_SIZE	0x10000
#define ARMV8MMUL3PAGEADDR(addr)	(((addr) >> 16) & 0xFFFFFFFF)
#define ARMV8MMUL3PAGEPTR(page)		((void *) ((page) << 16))

struct TARMV8MMU_LEVEL3_INVALID_DESCRIPTOR
{
	u64	Value0		: 1,		// set to 0
		Ignored		: 63;
}
__attribute__ ((packed));

union TARMV8MMU_LEVEL3_DESCRIPTOR
{
	TARMV8MMU_LEVEL3_PAGE_DESCRIPTOR	Page;
	TARMV8MMU_LEVEL3_INVALID_DESCRIPTOR	Invalid;
}
__attribute__ ((packed));

//CTranslationTable *m_pTranslationTable;
u64 m_nMemSize;
TARMV8MMU_LEVEL2_DESCRIPTOR* m_pTable; //= ( TARMV8MMU_LEVEL2_DESCRIPTOR** )0x0050018;

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

#define MEM_COHERENT_REGION	0x400000
#define MEM_HEAP_START		0x500000

void enable_mmu()
{
	u64 nMAIR_EL1 =   0xCC << ATTRINDX_NORMAL*8	// inner/outer write-back non-transient, non-allocating
	                | 0x04 << ATTRINDX_DEVICE*8	// Device-nGnRE
	                | 0x00 << ATTRINDX_COHERENT*8;	// Device-nGnRnE
	asm volatile ("msr mair_el1, %0" : : "r" (nMAIR_EL1));

	//asm volatile ("msr ttbr0_el1, %0" : : "r" (m_pTranslationTable->GetBaseAddress()));
	asm volatile ("msr ttbr0_el1, %0" : : "r" ((u64) (m_pTable)));

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

void *memset (void *pBuffer, int nValue, size_t nLength)
{
	char *p = (char *) pBuffer;

	while (nLength--)
	{
		*p++ = (char) nValue;
	}

	return pBuffer;
}

void *palloc (void)
{
	static unsigned char* s_pNextPage = ( unsigned char* )0x1b010000;
	void* pFreePage = (void*) s_pNextPage;
	s_pNextPage += 0x10000;
	return pFreePage;
}

TARMV8MMU_LEVEL3_DESCRIPTOR* __attribute__ ((optimize (0))) CreateLevel3Table (u64 nBaseAddress)
{
	TARMV8MMU_LEVEL3_DESCRIPTOR *pTable = (TARMV8MMU_LEVEL3_DESCRIPTOR *) palloc ();

	for (unsigned nPage = 0; nPage < ARMV8MMU_TABLE_ENTRIES; nPage++)	// 8192 entries a 64KB
	{
		TARMV8MMU_LEVEL3_PAGE_DESCRIPTOR *pDesc = &pTable[nPage].Page;

		pDesc->Value11	     = 3;
		pDesc->AttrIndx	     = ATTRINDX_NORMAL;
		pDesc->NS	     = 0;
		pDesc->AP	     = ATTRIB_AP_RW_EL1;
		pDesc->SH	     = ATTRIB_SH_INNER_SHAREABLE;
		pDesc->AF	     = 1;
		pDesc->nG	     = 0;
		pDesc->Reserved0_1   = 0;
		pDesc->OutputAddress = ARMV8MMUL3PAGEADDR (nBaseAddress);
		pDesc->Reserved0_2   = 0;
		pDesc->Continous     = 0;
		pDesc->PXN	     = 0;
		pDesc->UXN	     = 1;
		pDesc->Ignored	     = 0;

		extern u8 _etext;
		if (nBaseAddress >= (u64) &_etext)
		{
			pDesc->PXN = 1;

			if (nBaseAddress >= m_nMemSize)
			{
				pDesc->AttrIndx = ATTRINDX_DEVICE;
				pDesc->SH	= ATTRIB_SH_OUTER_SHAREABLE;
			}
			else if (   nBaseAddress >= MEM_COHERENT_REGION
				 && nBaseAddress <  MEM_HEAP_START)
			{
				pDesc->AttrIndx = ATTRINDX_COHERENT;
				pDesc->SH	= ATTRIB_SH_OUTER_SHAREABLE;
			}
		}

		nBaseAddress += ARMV8MMU_LEVEL3_PAGE_SIZE;
	}

	return pTable;
}

TARMV8MMU_LEVEL2_DESCRIPTOR* __attribute__ ((optimize (0))) createLevel2Table()
{
	TARMV8MMU_LEVEL2_DESCRIPTOR* ppTable;
	ppTable = (TARMV8MMU_LEVEL2_DESCRIPTOR *) palloc ();

	#define PAGE_SIZE		0x10000	
	memset (ppTable, 0, PAGE_SIZE);

	for (unsigned nEntry = 0; nEntry < 3; nEntry++)		// 3 entries a 512MB
	{
		u64 nBaseAddress = (u64) nEntry * ARMV8MMU_TABLE_ENTRIES * ARMV8MMU_LEVEL3_PAGE_SIZE;

		TARMV8MMU_LEVEL3_DESCRIPTOR *pTable = CreateLevel3Table (nBaseAddress);

		TARMV8MMU_LEVEL2_TABLE_DESCRIPTOR *pDesc = &ppTable[nEntry].Table;

		pDesc->Value11	    = 3;
		pDesc->Ignored1	    = 0;
		pDesc->TableAddress = ARMV8MMUL2TABLEADDR ((u64) pTable);
		pDesc->Reserved0    = 0;
		pDesc->Ignored2	    = 0;
		pDesc->PXNTable	    = 0;
		pDesc->UXNTable	    = 0;
		pDesc->APTable	    = AP_TABLE_ALL_ACCESS;
		pDesc->NSTable	    = 0;
	}

	//DataSyncBarrier ();
	asm volatile ("dsb sy" ::: "memory");
	return ppTable;
}

void initialize_memory()
{
	//mem_init ( 0, ( 512 - 64 ) * 0x100000 );

	//m_pTranslationTable = new CTranslationTable (m_nMemSize); //( CTranslationTable* )0x00500010;
	m_pTable = createLevel2Table();

	enable_mmu();
}

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

	u16 Palette[0];		// with Depth <= 8 only (256 entries)
#define PALETTE_ENTRIES		256
}
__attribute__ ((packed));

#define GPU_MEM_BASE	0xC0000000
#define ARM_IO_BASE		0x3F000000

#define ARM_SYSTIMER_BASE	(ARM_IO_BASE + 0x3000)
#define ARM_SYSTIMER_CLO	(ARM_SYSTIMER_BASE + 0x04)

#define MAILBOX_BASE		(ARM_IO_BASE + 0xB880)

#define MAILBOX0_READ  		(MAILBOX_BASE + 0x00)
#define MAILBOX0_STATUS 	(MAILBOX_BASE + 0x18)
	#define MAILBOX_STATUS_EMPTY	0x40000000
#define MAILBOX1_WRITE		(MAILBOX_BASE + 0x20)
#define MAILBOX1_STATUS 	(MAILBOX_BASE + 0x38)
	#define MAILBOX_STATUS_FULL	0x80000000

#define MAILBOX_CHANNEL_PM	0			// power management
#define MAILBOX_CHANNEL_FB 	1			// frame buffer
#define BCM_MAILBOX_PROP_OUT	8			// property tags (ARM to VC)

#define SETWAY_LEVEL_SHIFT		1

#define L1_DATA_CACHE_SETS		128
#define L1_DATA_CACHE_WAYS		4
	#define L1_SETWAY_WAY_SHIFT		30	// 32-Log2(L1_DATA_CACHE_WAYS)
#define L1_DATA_CACHE_LINE_LENGTH	64
	#define L1_SETWAY_SET_SHIFT		6	// Log2(L1_DATA_CACHE_LINE_LENGTH)

#define L2_CACHE_SETS			512
#define L2_CACHE_WAYS			16
	#define L2_SETWAY_WAY_SHIFT		28	// 32-Log2(L2_CACHE_WAYS)
#define L2_CACHE_LINE_LENGTH		64
	#define L2_SETWAY_SET_SHIFT		6	// Log2(L2_CACHE_LINE_LENGTH)

#define DATA_CACHE_LINE_LENGTH_MIN	64		// min(L1_DATA_CACHE_LINE_LENGTH, L2_CACHE_LINE_LENGTH)

int boot (void)
{
	initialize_memory();

	//CBcmMailBox m_MailBox(MAILBOX_CHANNEL_FB);
	volatile Bcm2835FrameBufferInfo* m_pInfo = ( Bcm2835FrameBufferInfo* )0x00500060; //new Bcm2835FrameBufferInfo;
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

	//CleanDataCache ();
	// clean L1 data cache
	for (register unsigned nSet = 0; nSet < L1_DATA_CACHE_SETS; nSet++)
	{
		for (register unsigned nWay = 0; nWay < L1_DATA_CACHE_WAYS; nWay++)
		{
			register u64 nSetWayLevel =   nWay << L1_SETWAY_WAY_SHIFT
						    | nSet << L1_SETWAY_SET_SHIFT
						    | 0 << SETWAY_LEVEL_SHIFT;

			asm volatile ("dc csw, %0" : : "r" (nSetWayLevel) : "memory");
		}
	}

	// clean L2 unified cache
	for (register unsigned nSet = 0; nSet < L2_CACHE_SETS; nSet++)
	{
		for (register unsigned nWay = 0; nWay < L2_CACHE_WAYS; nWay++)
		{
			register u64 nSetWayLevel =   nWay << L2_SETWAY_WAY_SHIFT
						    | nSet << L2_SETWAY_SET_SHIFT
						    | 1 << SETWAY_LEVEL_SHIFT;

			asm volatile ("dc csw, %0" : : "r" (nSetWayLevel) : "memory");
		}
	}

	//DataSyncBarrier ();
	asm volatile ("dsb sy" ::: "memory");

	//u32 nResult = m_MailBox.WriteRead (0xC0000000 + (u32) (u64) m_pInfo);
	//Flush ();
	while (!(*(u32 volatile *)(MAILBOX0_STATUS) & MAILBOX_STATUS_EMPTY))
	{
		*(u32 volatile *)(MAILBOX0_READ);

		//CTimer::SimpleMsDelay (20);
		#define CLOCKHZ	1000000
		u32 nTicks = 20 * 1000 * (CLOCKHZ / 1000000);
		u32 nStartTicks = *(u32 volatile *) (ARM_SYSTIMER_CLO);
		while(*(u32 volatile *)(ARM_SYSTIMER_CLO) - nStartTicks < nTicks);
	}

	//Write (0xC0000000 + (u32) (u64) m_pInfo);
	while (*(u32 volatile *) (MAILBOX1_STATUS) & MAILBOX_STATUS_FULL);
	*(u32 volatile *) (MAILBOX1_WRITE)= MAILBOX_CHANNEL_FB | (GPU_MEM_BASE + (u32) (u64) m_pInfo);	// channel number is in the lower 4 bits

	//u32 nResult = Read ();
	u32 nResult;
	do
	{
		while (*(u32 volatile *) (MAILBOX0_STATUS) & MAILBOX_STATUS_EMPTY);
		nResult = *(u32 volatile *) (MAILBOX0_READ);
	}
	while ((nResult & 0xF) != MAILBOX_CHANNEL_FB);		// channel number is in the lower 4 bits
	nResult = nResult & ~0xF;

	//InvalidateDataCache ();
	// invalidate L1 data cache
	for (register unsigned nSet = 0; nSet < L1_DATA_CACHE_SETS; nSet++)
	{
		for (register unsigned nWay = 0; nWay < L1_DATA_CACHE_WAYS; nWay++)
		{
			register u64 nSetWayLevel =   nWay << L1_SETWAY_WAY_SHIFT
						    | nSet << L1_SETWAY_SET_SHIFT
						    | 0 << SETWAY_LEVEL_SHIFT;

			asm volatile ("dc isw, %0" : : "r" (nSetWayLevel) : "memory");
		}
	}

	// invalidate L2 unified cache
	for (register unsigned nSet = 0; nSet < L2_CACHE_SETS; nSet++)
	{
		for (register unsigned nWay = 0; nWay < L2_CACHE_WAYS; nWay++)
		{
			register u64 nSetWayLevel =   nWay << L2_SETWAY_WAY_SHIFT
						    | nSet << L2_SETWAY_SET_SHIFT
						    | 1 << SETWAY_LEVEL_SHIFT;

			asm volatile ("dc isw, %0" : : "r" (nSetWayLevel) : "memory");
		}
	}

	//DataMemBarrier ();
	asm volatile ("dmb sy" ::: "memory");

	u32* buffer = (u32 *) (u64)( m_pInfo->BufferPtr & 0x3FFFFFFF );
	
	int color = 0x00ff0000;
	while( true )
	{
		for( u32 x = 0; x < 800; x++ )
			for( u32 y = 0; y < 480; y ++ )
				buffer[ 800 * y + x ] = color;
		color++;
		/*u64 address_0 = 0x1111111111111111;
		u64 address_1 = (u64) m_pTable;
		for( u64 digit = 0; digit < 63; digit++ )
		{
			if( ( address_0 & ( ( u64 )1 << digit ) ) == 0 )
			{
				for( u32 x = ( digit * 800 ) / 64; x < ( ( digit + 1 ) * 800 ) / 64; x++ )
					for( u32 y = 0; y < 240; y++ )
						buffer[ 800 * y + x ] = 0;
				for( u32 y = 0; y < 240; y++ )
						buffer[ 800 * y + ( ( digit + 1 ) * 800 ) / 64 - 1 ] = 0xff0000ff;
			}
			else
			{
				for( u32 x = ( digit * 800 ) / 64; x < ( ( digit + 1 ) * 800 ) / 64; x++ )
					for( u32 y = 0; y < 240; y++ )
						buffer[ 800 * y + x ] = 0xffffffff;
				for( u32 y = 0; y < 240; y++ )
						buffer[ 800 * y + ( ( digit + 1 ) * 800 ) / 64 - 1 ] = 0xff0000ff;
			}

			if( ( address_1 & ( ( u64 )1 << digit ) ) == 0 )
			{
				for( u32 x = ( digit * 800 ) / 64; x < ( ( digit + 1 ) * 800 ) / 64; x++ )
					for( u32 y = 240; y < 480; y++ )
						buffer[ 800 * y + x ] = 0;
				for( u32 y = 240; y < 480; y++ )
						buffer[ 800 * y + ( ( digit + 1 ) * 800 ) / 64 - 1 ] = 0xff0000ff;
			}
			else
			{
				for( u32 x = ( digit * 800 ) / 64; x < ( ( digit + 1 ) * 800 ) / 64; x++ )
					for( u32 y = 240; y < 480; y++ )
						buffer[ 800 * y + x ] = 0xffffffff;
				for( u32 y = 240; y < 480; y++ )
						buffer[ 800 * y + ( ( digit + 1 ) * 800 ) / 64 - 1 ] = 0xff0000ff;
			}
		}*/
	}
}
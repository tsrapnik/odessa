#include "type_definitions.h"
#include "mailbox_framebuffer.h"
#include "screen.h"
#include "buddy_heap.h"

int main();

#define ARMV8MMU_TABLE_ENTRIES		8192
//
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

void *memset (void *pBuffer, int nValue, u64 nLength)
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
	m_pTable = createLevel2Table();

	enable_mmu();
}

char a_string[20];
char* to_string(u64 number)
{
	for(u32 i = 0; i < 20; i++)
		a_string[i] = (u8)0;
	a_string[0] = '0';
	a_string[1] = 'x';
	u64 mask = 0x000000000000000f;
	
	for(i32 i = 15; i >= 0; i--)
	{
		u64 digit = (number >> (4*i)) & mask;
		switch(digit)
		{
			case 0x0:
				a_string[2+15-i] = '0';
				break;
			case 0x1:
				a_string[2+15-i] = '1';
				break;
			case 0x2:
				a_string[2+15-i] = '2';
				break;
			case 0x3:
				a_string[2+15-i] = '3';
				break;
			case 0x4:
				a_string[2+15-i] = '4';
				break;
			case 0x5:
				a_string[2+15-i] = '5';
				break;
			case 0x6:
				a_string[2+15-i] = '6';
				break;
			case 0x7:
				a_string[2+15-i] = '7';
				break;
			case 0x8:
				a_string[2+15-i] = '8';
				break;
			case 0x9:
				a_string[2+15-i] = '9';
				break;
			case 0xa:
				a_string[2+15-i] = 'a';
				break;
			case 0xb:
				a_string[2+15-i] = 'b';
				break;
			case 0xc:
				a_string[2+15-i] = 'c';
				break;
			case 0xd:
				a_string[2+15-i] = 'd';
				break;
			case 0xe:
				a_string[2+15-i] = 'e';
				break;
			case 0xf:
				a_string[2+15-i] = 'f';
				break;
			default:
				break;
		}
	}
	return a_string;
}

extern "C" void initialize (void)
{
	//clear bss.
	extern unsigned char __bss_start;
	extern unsigned char _end;
	for (unsigned char *pBSS = &__bss_start; pBSS < &_end; pBSS++)
	{
		*pBSS = 0;
	}

	//call construtors of static objects.
	extern void (*__init_start) (void);
	extern void (*__init_end) (void);
	for (void (**pFunc) (void) = &__init_start; pFunc < &__init_end; pFunc++)
	{
		(**pFunc) ();
	}
	
	initialize_memory();
	buddy_heap::initialize();
	mailbox_framebuffer a_mailbox_framebuffer;
	screen a_screen(vector_2_int(800,480), reinterpret_cast<colour*>(a_mailbox_framebuffer.get_framebuffer()));
	a_screen.clear(colour(0,255,0,255));

	u32* test = new u32();
	a_screen.draw_text(to_string((u64)test), vector_2_int(0,21));
	a_screen.draw_text("test", vector_2_int(0,42));
	while( true )
	{
	}
	main();
}
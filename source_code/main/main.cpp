#include "mailbox_framebuffer.h"
/*
CTranslationTable *m_pTranslationTable;
u64 m_nMemSize;

void enable_mmu()
{
	//assert (m_bEnableMMU);

	u64 nMAIR_EL1 =   0xCC << 0 * 8 //ATTRINDX_NORMAL*8	// inner/outer write-back non-transient, non-allocating
	                | 0x04 << 1 * 8 //ATTRINDX_DEVICE*8	// Device-nGnRE
	                | 0x00 << 2 * 8; //ATTRINDX_COHERENT*8;	// Device-nGnRnE
	asm volatile ("msr mair_el1, %0" : : "r" (nMAIR_EL1));

	assert (m_pTranslationTable != 0);
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
	CBcmPropertyTags Tags;
	TPropertyTagMemory TagMemory;
	if (!Tags.GetTag (PROPTAG_GET_ARM_MEMORY, &TagMemory, sizeof TagMemory))
	{
		TagMemory.nBaseAddress = 0;
		TagMemory.nSize = ARM_MEM_SIZE;
	}

	assert (TagMemory.nBaseAddress == 0);
	m_nMemSize = TagMemory.nSize;

	mem_init (TagMemory.nBaseAddress, m_nMemSize);

	m_pTranslationTable = new CTranslationTable (m_nMemSize);
	enable_mmu();
}*/

//Main function
int main( void )
{
	//initialize_memory();
	mailbox_framebuffer this_mailbox_framebuffer;
	volatile u32* buffer  = reinterpret_cast< volatile u32* >( this_mailbox_framebuffer.this_mailbox_framebuffer_info->buffer_pointer );
	for( u32 i = 0; i < 480 * 800; i ++ )
		buffer[ i ] = 0xff00ff00;
	return 0;
}

/* Dummy functions to avoid linker complaints */
extern "C" void __cxa_pure_virtual()
{
	while( 1 );
}

extern "C" void __aeabi_atexit( void )
{
	while( 1 );
}

extern "C" void __dso_handle( void )
{
	while( 1 );
}

extern "C" void __cxa_guard_acquire( void )
{}

extern "C" void __cxa_guard_release( void )
{}

extern "C" void __cxa_atexit( void )
{}
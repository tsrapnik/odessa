#ifndef _circle_startup_h
#define _circle_startup_h

#ifndef _circle_sysconfig_h
#define _circle_sysconfig_h

// system options

// memory addresses and sizes
#define MEGABYTE		0x100000

#define MEM_SIZE		(512 * MEGABYTE)		// default size
#define GPU_MEM_SIZE		(64 * MEGABYTE)			// set in config.txt
#define ARM_MEM_SIZE		(MEM_SIZE - GPU_MEM_SIZE)	// normally overwritten

#define PAGE_SIZE		0x10000				// page size used by us

#define KERNEL_MAX_SIZE		(2 * MEGABYTE)
#define KERNEL_STACK_SIZE	0x20000
#define EXCEPTION_STACK_SIZE	0x8000
#define PAGE_RESERVE		(16 * MEGABYTE)

#define MEM_KERNEL_START	0x80000						// main code starts here
#define MEM_KERNEL_END		(MEM_KERNEL_START + KERNEL_MAX_SIZE)
#define MEM_KERNEL_STACK	(MEM_KERNEL_END + KERNEL_STACK_SIZE)		// expands down
#define CORES			4						// must be a power of 2
#define MEM_EXCEPTION_STACK	(MEM_KERNEL_STACK + KERNEL_STACK_SIZE * (CORES-1) + EXCEPTION_STACK_SIZE)

#define MEM_COHERENT_REGION	0x400000
#define MEM_HEAP_START		0x500000

#endif

#ifndef _circle_types_h
#define _circle_types_h

typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned long		u64;

typedef signed char		s8;
typedef signed short		s16;
typedef signed int		s32;
typedef signed long		s64;

typedef long			intptr;
typedef unsigned long		uintptr;

typedef int		boolean;
#define FALSE		0
#define TRUE		1

typedef unsigned long	size_t;
typedef long		ssize_t;

#endif

#define EXIT_HALT	0
#define EXIT_REBOOT	1

extern "C" void halt (void);
extern "C" void reboot (void);

struct TSpinTable
{
	uintptr SpinCore[CORES];
};

#define ARM_SPIN_TABLE_BASE	0x000000D8

void _start_secondary (void);
void main_secondary (void);


#endif

static inline u32 read32 (uintptr nAddress)
{
	return *(u32 volatile *) nAddress;
}

static inline void write32 (uintptr nAddress, u32 nValue)
{
	*(u32 volatile *) nAddress = nValue;
}

#ifndef _circle_bcm2835_h
#define _circle_bcm2835_h

#if RASPPI == 1
#define ARM_IO_BASE		0x20000000
#else
#define ARM_IO_BASE		0x3F000000
#endif

#define GPU_IO_BASE		0x7E000000

#define GPU_CACHED_BASE		0x40000000
#define GPU_UNCACHED_BASE	0xC0000000

#if RASPPI == 1
	#ifdef GPU_L2_CACHE_ENABLED
		#define GPU_MEM_BASE	GPU_CACHED_BASE
	#else
		#define GPU_MEM_BASE	GPU_UNCACHED_BASE
	#endif
#else
	#define GPU_MEM_BASE	GPU_UNCACHED_BASE
#endif

//
// General Purpose I/O
//
#define ARM_GPIO_BASE		(ARM_IO_BASE + 0x200000)

#define ARM_GPIO_GPFSEL0	(ARM_GPIO_BASE + 0x00)
#define ARM_GPIO_GPFSEL1	(ARM_GPIO_BASE + 0x04)
#define ARM_GPIO_GPFSEL4	(ARM_GPIO_BASE + 0x10)
#define ARM_GPIO_GPSET0		(ARM_GPIO_BASE + 0x1C)
#define ARM_GPIO_GPCLR0		(ARM_GPIO_BASE + 0x28)
#define ARM_GPIO_GPLEV0		(ARM_GPIO_BASE + 0x34)
#define ARM_GPIO_GPEDS0		(ARM_GPIO_BASE + 0x40)
#define ARM_GPIO_GPREN0		(ARM_GPIO_BASE + 0x4C)
#define ARM_GPIO_GPFEN0		(ARM_GPIO_BASE + 0x58)
#define ARM_GPIO_GPHEN0		(ARM_GPIO_BASE + 0x64)
#define ARM_GPIO_GPLEN0		(ARM_GPIO_BASE + 0x70)
#define ARM_GPIO_GPAREN0	(ARM_GPIO_BASE + 0x7C)
#define ARM_GPIO_GPAFEN0	(ARM_GPIO_BASE + 0x88)
#define ARM_GPIO_GPPUD		(ARM_GPIO_BASE + 0x94)
#define ARM_GPIO_GPPUDCLK0	(ARM_GPIO_BASE + 0x98)

//
// UART0
//
#define ARM_UART0_BASE		(ARM_IO_BASE + 0x201000)

#define ARM_UART0_DR		(ARM_UART0_BASE + 0x00)
#define ARM_UART0_FR     	(ARM_UART0_BASE + 0x18)
#define ARM_UART0_IBRD   	(ARM_UART0_BASE + 0x24)
#define ARM_UART0_FBRD   	(ARM_UART0_BASE + 0x28)
#define ARM_UART0_LCRH   	(ARM_UART0_BASE + 0x2C)
#define ARM_UART0_CR     	(ARM_UART0_BASE + 0x30)
#define ARM_UART0_IFLS   	(ARM_UART0_BASE + 0x34)
#define ARM_UART0_IMSC   	(ARM_UART0_BASE + 0x38)
#define ARM_UART0_RIS    	(ARM_UART0_BASE + 0x3C)
#define ARM_UART0_MIS    	(ARM_UART0_BASE + 0x40)
#define ARM_UART0_ICR    	(ARM_UART0_BASE + 0x44)

//
// System Timers
//
#define ARM_SYSTIMER_BASE	(ARM_IO_BASE + 0x3000)

#define ARM_SYSTIMER_CS		(ARM_SYSTIMER_BASE + 0x00)
#define ARM_SYSTIMER_CLO	(ARM_SYSTIMER_BASE + 0x04)
#define ARM_SYSTIMER_CHI	(ARM_SYSTIMER_BASE + 0x08)
#define ARM_SYSTIMER_C0		(ARM_SYSTIMER_BASE + 0x0C)
#define ARM_SYSTIMER_C1		(ARM_SYSTIMER_BASE + 0x10)
#define ARM_SYSTIMER_C2		(ARM_SYSTIMER_BASE + 0x14)
#define ARM_SYSTIMER_C3		(ARM_SYSTIMER_BASE + 0x18)

//
// Platform DMA Controller
//
#define ARM_DMA_BASE		(ARM_IO_BASE + 0x7000)

//
// Interrupt Controller
//
#define ARM_IC_BASE		(ARM_IO_BASE + 0xB000)

#define ARM_IC_IRQ_BASIC_PENDING  (ARM_IC_BASE + 0x200)
#define ARM_IC_IRQ_PENDING_1	  (ARM_IC_BASE + 0x204)
#define ARM_IC_IRQ_PENDING_2	  (ARM_IC_BASE + 0x208)
#define ARM_IC_FIQ_CONTROL	  (ARM_IC_BASE + 0x20C)
#define ARM_IC_ENABLE_IRQS_1	  (ARM_IC_BASE + 0x210)
#define ARM_IC_ENABLE_IRQS_2	  (ARM_IC_BASE + 0x214)
#define ARM_IC_ENABLE_BASIC_IRQS  (ARM_IC_BASE + 0x218)
#define ARM_IC_DISABLE_IRQS_1	  (ARM_IC_BASE + 0x21C)
#define ARM_IC_DISABLE_IRQS_2	  (ARM_IC_BASE + 0x220)
#define ARM_IC_DISABLE_BASIC_IRQS (ARM_IC_BASE + 0x224)

//
// ARM Timer
//
#define ARM_TIMER_BASE		(ARM_IO_BASE + 0xB000)

#define ARM_TIMER_LOAD		(ARM_TIMER_BASE + 0x400)
#define ARM_TIMER_VALUE		(ARM_TIMER_BASE + 0x404)
#define ARM_TIMER_CTRL		(ARM_TIMER_BASE + 0x408)
#define ARM_TIMER_IRQCLR	(ARM_TIMER_BASE + 0x40C)
#define ARM_TIMER_RAWIRQ	(ARM_TIMER_BASE + 0x410)
#define ARM_TIMER_MASKIRQ	(ARM_TIMER_BASE + 0x414)
#define ARM_TIMER_RELOAD	(ARM_TIMER_BASE + 0x418)
#define ARM_TIMER_PREDIV	(ARM_TIMER_BASE + 0x41C)
#define ARM_TIMER_CNTR		(ARM_TIMER_BASE + 0x420)

//
// Mailbox
//
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

//
// Pulse Width Modulator
//
#define ARM_PWM_BASE		(ARM_IO_BASE + 0x20C000)

#define ARM_PWM_CTL		(ARM_PWM_BASE + 0x00)
#define ARM_PWM_STA		(ARM_PWM_BASE + 0x04)
#define ARM_PWM_DMAC		(ARM_PWM_BASE + 0x08)
#define ARM_PWM_RNG1		(ARM_PWM_BASE + 0x10)
#define ARM_PWM_DAT1		(ARM_PWM_BASE + 0x14)
#define ARM_PWM_FIF1		(ARM_PWM_BASE + 0x18)
#define ARM_PWM_RNG2		(ARM_PWM_BASE + 0x20)
#define ARM_PWM_DAT2		(ARM_PWM_BASE + 0x24)

//
// Clock Manager
//
#define ARM_CM_BASE		(ARM_IO_BASE + 0x101000)

#define ARM_CM_GP0CTL		(ARM_CM_BASE + 0x70)
#define ARM_CM_GP0DIV		(ARM_CM_BASE + 0x74)

#define ARM_CM_PASSWD 		(0x5A << 24)

//
// USB Host Controller
//
#define ARM_USB_BASE		(ARM_IO_BASE + 0x980000)

#define ARM_USB_CORE_BASE	ARM_USB_BASE
#define ARM_USB_HOST_BASE	(ARM_USB_BASE + 0x400)
#define ARM_USB_POWER		(ARM_USB_BASE + 0xE00)

//
// External Mass Media Controller (SD Card)
//
#define ARM_EMMC_BASE		(ARM_IO_BASE + 0x300000)

//
// Power Manager (?)
//
#define ARM_PM_BASE		(ARM_IO_BASE + 0x100000)

#define ARM_PM_RSTC		(ARM_PM_BASE + 0x1C)
#define ARM_PM_WDOG		(ARM_PM_BASE + 0x24)

#define ARM_PM_PASSWD		(0x5A << 24)

//
// BSC Master
//
#define ARM_BSC0_BASE		(ARM_IO_BASE + 0x205000)
#define ARM_BSC1_BASE		(ARM_IO_BASE + 0x804000)

#define ARM_BSC_C__OFFSET	0x00
#define ARM_BSC_S__OFFSET	0x04
#define ARM_BSC_DLEN__OFFSET	0x08
#define ARM_BSC_A__OFFSET	0x0C
#define ARM_BSC_FIFO__OFFSET	0x10
#define ARM_BSC_DIV__OFFSET	0x14
#define ARM_BSC_DEL__OFFSET	0x18
#define ARM_BSC_CLKT__OFFSET	0x1C

//
// BSC / SPI Slave
//
#define ARM_BSC_SPI_SLAVE_BASE	(ARM_IO_BASE + 0x214000)

#define ARM_BSC_SPI_SLAVE_DR	(ARM_BSC_SPI_SLAVE_BASE + 0x00)
#define ARM_BSC_SPI_SLAVE_RSR	(ARM_BSC_SPI_SLAVE_BASE + 0x04)
#define ARM_BSC_SPI_SLAVE_SLV	(ARM_BSC_SPI_SLAVE_BASE + 0x08)
#define ARM_BSC_SPI_SLAVE_CR	(ARM_BSC_SPI_SLAVE_BASE + 0x0C)
#define ARM_BSC_SPI_SLAVE_FR	(ARM_BSC_SPI_SLAVE_BASE + 0x10)
#define ARM_BSC_SPI_SLAVE_IFLS	(ARM_BSC_SPI_SLAVE_BASE + 0x14)
#define ARM_BSC_SPI_SLAVE_IMSC	(ARM_BSC_SPI_SLAVE_BASE + 0x18)
#define ARM_BSC_SPI_SLAVE_RIS	(ARM_BSC_SPI_SLAVE_BASE + 0x1C)
#define ARM_BSC_SPI_SLAVE_MIS	(ARM_BSC_SPI_SLAVE_BASE + 0x20)
#define ARM_BSC_SPI_SLAVE_ICR	(ARM_BSC_SPI_SLAVE_BASE + 0x24)

#endif
#ifndef _circle_synchronize_h
#define _circle_synchronize_h

#ifndef _circle_macros_h
#define _circle_macros_h

#define PACKED		__attribute__ ((packed))
#define	ALIGN(n)	__attribute__ ((aligned (n)))
#define NOOPT		__attribute__ ((optimize (0)))
#define MAXOPT		__attribute__ ((optimize (3)))

// big endian (to be used for constants only)
#define BE(value)	((((value) & 0xFF00) >> 8) | (((value) & 0x00FF) << 8))

#endif

#ifdef __cplusplus
extern "C" {
#endif

//
// Interrupt control
//
#define	EnableInterrupts()	asm volatile ("msr DAIFClr, #2")
#define	DisableInterrupts()	asm volatile ("msr DAIFSet, #2")

void EnterCritical (void);
void LeaveCritical (void);

//
// Cache control
//
#define InvalidateInstructionCache()	asm volatile ("ic iallu" ::: "memory")
#define FlushPrefetchBuffer()		asm volatile ("isb" ::: "memory")

void InvalidateDataCache (void) MAXOPT;
void InvalidateDataCacheL1Only (void) MAXOPT;
void CleanDataCache (void) MAXOPT;

void InvalidateDataCacheRange (u64 nAddress, u64 nLength) MAXOPT;
void CleanDataCacheRange (u64 nAddress, u64 nLength) MAXOPT;
void CleanAndInvalidateDataCacheRange (u64 nAddress, u64 nLength) MAXOPT;

//
// Barriers
//
#define DataSyncBarrier()	asm volatile ("dsb sy" ::: "memory")
#define DataMemBarrier() 	asm volatile ("dmb sy" ::: "memory")

#define InstructionSyncBarrier() asm volatile ("isb" ::: "memory")
#define InstructionMemBarrier()	asm volatile ("isb" ::: "memory")

#define CompilerBarrier()	asm volatile ("" ::: "memory")

#ifdef __cplusplus
}
#endif

#endif

void halt (void)
{ 
	static volatile boolean s_bCoreHalted[CORES] = {FALSE};

	u64 nMPIDR;
	asm volatile ("mrs %0, mpidr_el1" : "=r" (nMPIDR));
	unsigned nCore = nMPIDR & (CORES-1);

	// core 0 must not halt until all secondary cores have been halted
	if (nCore == 0)
	{
		unsigned nSecCore = 1;
		while (nSecCore < CORES)
		{
			DataMemBarrier ();
			if (!s_bCoreHalted[nSecCore])
			{
				DataSyncBarrier ();
				asm volatile ("wfi");

				nSecCore = 1;
			}
			else
			{
				nSecCore++;
			}
		}
	}

	s_bCoreHalted[nCore] = TRUE;
	DataMemBarrier ();

	DisableInterrupts ();

	DataSyncBarrier ();

	for (;;)
	{
		asm volatile ("wfi");
	}
}

void reboot (void)					// by PlutoniumBob@raspi-forum
{
	write32 (ARM_PM_WDOG, ARM_PM_PASSWD | 1);	// set some timeout

	#define PM_RSTC_WRCFG_FULL_RESET	0x20
	write32 (ARM_PM_RSTC, ARM_PM_PASSWD | PM_RSTC_WRCFG_FULL_RESET);

	for (;;);					// wait for reset
}

extern "C" void sysinit (void)
{
	// clear BSS
	extern unsigned char __bss_start;
	extern unsigned char _end;
	for (unsigned char *pBSS = &__bss_start; pBSS < &_end; pBSS++)
	{
		*pBSS = 0;
	}

	// call construtors of static objects
	extern void (*__init_start) (void);
	extern void (*__init_end) (void);
	for (void (**pFunc) (void) = &__init_start; pFunc < &__init_end; pFunc++)
	{
		(**pFunc) ();
	}

	extern int main (void);
	if (main () == EXIT_REBOOT)
	{
		reboot ();
	}

	halt ();
}

extern "C" void sysinit_secondary (void)
{
	//main_secondary ();

	halt ();
}

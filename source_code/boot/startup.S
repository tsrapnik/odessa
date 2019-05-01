<<<<<<< HEAD
#define kernel_maximum_size 0x200000
#define memory_kernel_start	0x80000
#define exception_stack_size 0x8000
#define kernel_stack_size 0x20000
#define memory_kernel_end (memory_kernel_start + kernel_maximum_size)
#define memory_kernel_stack (memory_kernel_end + kernel_stack_size)
#define memory_exception_stack (memory_kernel_stack + exception_stack_size)

	.macro armv8_switch_to_el1_m, xreg1, xreg2

	/* Initialize Generic Timers */
	mrs	\xreg1, cnthctl_el2
	orr	\xreg1, \xreg1, #0x3	/* Enable EL1 access to timers */
	msr	cnthctl_el2, \xreg1
	msr	cntvoff_el2, xzr

	/* Initilize MPID/MPIDR registers */
	mrs	\xreg1, midr_el1
	mrs	\xreg2, mpidr_el1
	msr	vpidr_el2, \xreg1
	msr	vmpidr_el2, \xreg2

	/* Disable coprocessor traps */
	mov	\xreg1, #0x33ff
	msr	cptr_el2, \xreg1	/* Disable coprocessor traps to EL2 */
	msr	hstr_el2, xzr		/* Disable coprocessor traps to EL2 */
	mov	\xreg1, #3 << 20
	msr	cpacr_el1, \xreg1	/* Enable FP/SIMD at EL1 */

	/* Initialize HCR_EL2 */
	mov	\xreg1, #(1 << 31)		/* 64bit EL1 */
	msr	hcr_el2, \xreg1

	/* SCTLR_EL1 initialization
	 *
	 * setting RES1 bits (29,28,23,22,20,11) to 1
	 * and RES0 bits (31,30,27,21,17,13,10,6) +
	 * UCI,EE,EOE,WXN,nTWE,nTWI,UCT,DZE,I,UMA,SED,ITD,
	 * CP15BEN,SA0,SA,C,A,M to 0
	 */
	mov	\xreg1, #0x0800
	movk	\xreg1, #0x30d0, lsl #16
	msr	sctlr_el1, \xreg1

	/* Return to the EL1_SP1 mode from EL2 */
	mov	\xreg1, #0x3c4
	msr	spsr_el2, \xreg1	/* EL1_SP0 | D | A | I | F */
	adr	\xreg1, 1f
	msr	elr_el2, \xreg1
	eret
1:

	.endm

	.text

	.globl	_start
_start:					/* normally entered from armstub8 in EL2 after boot */

	mrs	x0, CurrentEL		/* check if already in EL1t mode? */
	cmp	x0, #4
	beq	1f

	ldr	x0, =memory_exception_stack /* IRQ and exception handler run in EL1h */
	msr	sp_el1, x0		/* init their stack */

	armv8_switch_to_el1_m x0, x1

1:	ldr	x0, =memory_kernel_stack	/* main thread runs in EL1t and uses sp_el0 */
	mov	sp, x0			/* init its stack */

	ldr	x0, =vector_table	/* init exception vector table */
	msr	vbar_el1, x0

	b	main

	.text

	.align	11

	.globl	vector_table
vector_table:

	/* from current EL with sp_el0 */
	b	exception_handler
	b	interrupt_handler
	b	exception_handler			/* FIQ */
	b	exception_handler

	/* from current EL with sp_elx, x != 0 */
	b	exception_handler
	b	exception_handler
	b	exception_handler
	b	exception_handler
	
	/* from lower EL, target EL minus 1 is AArch64 */
	b	exception_handler
	b	exception_handler
	b	exception_handler
	b	exception_handler

	/* from lower EL, target EL minus 1 is AArch32 */
	b	exception_handler
	b	exception_handler
	b	exception_handler
	b	exception_handler

	.globl exception_handler
	exception_handler:
	b exception_handler
	
	.globl interrupt_handler
	interrupt_handler:
	b	interrupt_handler     
=======
.text
.balign	4
.globl _start
_start:
    ldr x2, = __stack_core_0_el2				// Address of EL2_stack_core0 stack pointer value
    ldr x3, = __stack_core_0_el1				// Address of EL1_stack_core0 stack pointer value
    ldr x4, = __stack_core_0_el0				// Address of EL0_stack_core0 stack pointer value
	mov	sp, x2								/* EL2 stack set */
	msr	sp_el1, x3							/* EL1 stack set */
	msr	sp_el0, x4							/* EL0 stack set */

	/* Initialize Generic Timers */
	mrs	x0, cnthctl_el2
	orr	x0, x0, #0x3	/* Enable EL1 access to timers */
	msr	cnthctl_el2, x0
	msr	cntvoff_el2, xzr

	/* Initilize MPID/MPIDR registers */
	mrs	x0, midr_el1
	mrs	x1, mpidr_el1
	msr	vpidr_el2, x0
	msr	vmpidr_el2, x1

	mov	x0, #0x33ff
	msr	cptr_el2, x0						// Disable coprocessor traps to EL2
	msr	hstr_el2, xzr						// Disable coprocessor traps to EL2
	mov	x0, #3 << 20
	msr	cpacr_el1, x0						// Enable FP/SIMD at EL1

	mov	x0, #(1 << 31)						// 64bit EL1
	msr	hcr_el2, x0

	mov	x0, #0x0800
	movk	x0, #0x30d0, lsl #16
	orr    x0, x0, #(0x1 << 2)            // The C bit on (data cache). 
	orr    x0, x0, #(0x1 << 12)           // The I bit on (instruction cache)
	msr	sctlr_el1, x0

	mov	x0, #0x3c5							// EL1_SP1 | D | A | I | F
	msr	spsr_el2, x0						// Set spsr_el2 with settings
	adr	x0, exit_el1						// Address to exit EL2
	msr	elr_el2, x0							// Set elevated return register
	eret									// Call elevated return
exit_el1:

    ldr x0, =VectorTable						
    msr vbar_el1,x0

	ldr x3, =__bss_end
	ldr x0, =__bss_start
	cmp	x0, x3
	bcs	.bss_cleared
.bss_zero_loop:
	str	wzr, [x0], 4
	cmp	x3, x0
	bhi	.bss_zero_loop
.bss_cleared:

	b main							// Jump out to C kernel 

hang:
	b hang

.macro	vector handler
.balign 0x80
b	\handler
.endm

.balign 0x800
.globl	VectorTable
VectorTable:
	/* from current EL with sp_el0 */
	vector	syn_cur_el0			/* Synchronous */
	vector  irq_cur_el0			/* IRQ */
	vector	fiq_cur_el0			/* FIQ */
	vector	err_cur_el0			/* SErrorStub */

	/* from current EL with sp_elx, x != 0 */
	vector	syn_cur_elx			 /* Synchronous */
	vector	irq_cur_elx /* IRQ */
	vector	fiq_cur_elx /* FIQ */
	vector	err_cur_elx			 /* SErrorStub */

	/* from lower EL, target EL minus 1 is AArch64 */
	vector	syn_low64_elx			/* Synchronous */
	vector  irq_low64_elx			/* IRQ */
	vector	fiq_low64_elx			/* FIQ */
	vector	err_low64_elx			/* SErrorStub */

	/* from lower EL, target EL minus 1 is AArch32 */
	vector	syn_low32_elx			/* Synchronous */
	vector  irq_low32_elx			/* IRQ */
	vector	fiq_low32_elx			/* FIQ */
	vector	err_low32_elx			/* SErrorStub */
>>>>>>> include_gpu

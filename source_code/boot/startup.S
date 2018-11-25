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

.weak irq_handler_stub
irq_handler_stub:
	eret

.weak fiq_handler_stub
fiq_handler_stub:
	eret

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

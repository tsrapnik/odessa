.text
.balign    4
.globl _start
_start:
    ldr x2, = __stack_core_0_el2                // Address of EL2_stack_core0 stack pointer value
    ldr x3, = __stack_core_0_el1                // Address of EL1_stack_core0 stack pointer value
    ldr x4, = __stack_core_0_el0                // Address of EL0_stack_core0 stack pointer value
    mov    sp, x2                                /* EL2 stack set */
    msr    sp_el1, x3                            /* EL1 stack set */
    msr    sp_el0, x4                            /* EL0 stack set */

    /* Initialize Generic Timers */
    mrs    x0, cnthctl_el2
    orr    x0, x0, #0x3    /* Enable EL1 access to timers */
    msr    cnthctl_el2, x0
    msr    cntvoff_el2, xzr

    /* Initilize MPID/MPIDR registers */
    mrs    x0, midr_el1
    mrs    x1, mpidr_el1
    msr    vpidr_el2, x0
    msr    vmpidr_el2, x1

    mov    x0, #0x33ff
    msr    cptr_el2, x0                        // Disable coprocessor traps to EL2
    msr    hstr_el2, xzr                        // Disable coprocessor traps to EL2
    mov    x0, #3 << 20
    msr    cpacr_el1, x0                        // Enable FP/SIMD at EL1

    mov    x0, #(1 << 31)                        // 64bit EL1
    msr    hcr_el2, x0

    mov    x0, #0x0800
    movk    x0, #0x30d0, lsl #16
    orr    x0, x0, #(0x1 << 2)            // The C bit on (data cache). 
    orr    x0, x0, #(0x1 << 12)           // The I bit on (instruction cache)
    msr    sctlr_el1, x0

    mov    x0, #0x345                            // EL1_SP1 | D | A | F
    msr    spsr_el2, x0                        // Set spsr_el2 with settings
    adr    x0, exit_el1                        // Address to exit EL2
    msr    elr_el2, x0                            // Set elevated return register
    eret                                    // Call elevated return
exit_el1:

    ldr x0, =VectorTable                        
    msr vbar_el1,x0

    b main                            // Jump out to C kernel 

hang:
    b hang

.macro    vector handler
.balign 0x80
b    \handler
.endm

.balign 0x800
.globl    VectorTable
VectorTable:
    /* from current EL with sp_el0 */
    vector    syn_cur_el0            /* Synchronous */
    vector  irq_cur_el0            /* IRQ */
    vector    fiq_cur_el0            /* FIQ */
    vector    err_cur_el0            /* SErrorStub */

    /* from current EL with sp_elx, x != 0 */
    vector    syn_cur_elx             /* Synchronous */
    vector    irq_cur_elx_wrapper /* IRQ */
    vector    fiq_cur_elx /* FIQ */
    vector    err_cur_elx             /* SErrorStub */

    /* from lower EL, target EL minus 1 is AArch64 */
    vector    syn_low64_elx            /* Synchronous */
    vector  irq_low64_elx            /* IRQ */
    vector    fiq_low64_elx            /* FIQ */
    vector    err_low64_elx            /* SErrorStub */

    /* from lower EL, target EL minus 1 is AArch32 */
    vector    syn_low32_elx            /* Synchronous */
    vector  irq_low32_elx            /* IRQ */
    vector    fiq_low32_elx            /* FIQ */
    vector    err_low32_elx            /* SErrorStub */

//makes sure the interrupt handler correctly returns to regular code.
irq_cur_elx_wrapper:
    stp    x29, x30, [sp, #-16]!        /* save x29, x30 onto stack */

    mrs    x29, elr_el1            /* save elr_el1, spsr_el1 onto stack */
    mrs    x30, spsr_el1
    stp    x29, x30, [sp, #-16]!

    stp    x27, x28, [sp, #-16]!        /* save x0-x28 onto stack */
    stp    x25, x26, [sp, #-16]!
    stp    x23, x24, [sp, #-16]!
    stp    x21, x22, [sp, #-16]!
    stp    x19, x20, [sp, #-16]!
    stp    x17, x18, [sp, #-16]!
    stp    x15, x16, [sp, #-16]!
    stp    x13, x14, [sp, #-16]!
    stp    x11, x12, [sp, #-16]!
    stp    x9, x10, [sp, #-16]!
    stp    x7, x8, [sp, #-16]!
    stp    x5, x6, [sp, #-16]!
    stp    x3, x4, [sp, #-16]!
    stp    x1, x2, [sp, #-16]!
    str    x0, [sp, #-16]!

    bl    irq_cur_elx

    ldr    x0, [sp], #16            /* restore x0-x28 from stack */
    ldp    x1, x2, [sp], #16
    ldp    x3, x4, [sp], #16
    ldp    x5, x6, [sp], #16
    ldp    x7, x8, [sp], #16
    ldp    x9, x10, [sp], #16
    ldp    x11, x12, [sp], #16
    ldp    x13, x14, [sp], #16
    ldp    x15, x16, [sp], #16
    ldp    x17, x18, [sp], #16
    ldp    x19, x20, [sp], #16
    ldp    x21, x22, [sp], #16
    ldp    x23, x24, [sp], #16
    ldp    x25, x26, [sp], #16
    ldp    x27, x28, [sp], #16

    ldp    x29, x30, [sp], #16        /* restore elr_el1, spsr_el1 from stack */
    msr    elr_el1, x29
    msr    spsr_el1, x30

    ldp    x29, x30, [sp], #16        /* restore x29, x30 from stack */

    eret

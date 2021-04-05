.section .text.start
.balign 4
_start:
    // core 0 starts here. this is initially the only core that is awake.

    // a spin table is defined in memory, where pointers are stored where the other cores should start. we
    // let all cores start with the init_core function. address for pointers are found in
    // https://github.com/raspberrypi/tools/blob/master/armstubs/armstub8.S. however we do not start the other
    // cores yet, because we want core 0 to initialize some things in c++ code first (e.g. initializing static
    // variables). afterwards we let core 0 start the other cores from c++ code.
    ldr x0, =init_core
    mov x1, #0xd8
    str x0, [x1, #8]!
    str x0, [x1, #8]!
    str x0, [x1, #8]!

    // now initialize core 0.
    b init_core

// function initializes core and calls main c++ function afterwards.
init_core:
    // code starts in el2. we do some initialization in el2 and then return to el1 for the rest of our code.

    // get core index.
    mrs x0, mpidr_el1
    and x0, x0, #0x3

    // set stack pointers for el2 to el0, depending on core index.
    ldr x1, =stack_el2
    ldr x2, [x1, x0, lsl #3]
    ldr x1, =stack_el1
    ldr x3, [x1, x0, lsl #3]
    ldr x1, =stack_el0
    ldr x4, [x1, x0, lsl #3]
    mov sp, x2
    msr sp_el1, x3
    msr sp_el0, x4

    // init generic timers.
    mrs x1, cnthctl_el2
    orr x1, x1, #0x3    // enable el1 access to timers.
    msr cnthctl_el2, x1
    msr cntvoff_el2, xzr

    // initialize mpid and mpidr registers.
    mrs x1, midr_el1
    mrs x2, mpidr_el1
    msr vpidr_el2, x1
    msr vmpidr_el2, x2

    mov x1, #0x33ff
    msr cptr_el2, x1 // disable coprocessor traps to el2.
    msr hstr_el2, xzr // disable coprocessor traps to el2.
    mov x1, #3 << 20
    msr cpacr_el1, x1 // enable fp and simd at el1.

    mov x1, #(1 << 31) // enable 64bit in el1.
    msr hcr_el2, x1

    // spsr_el2 will be copied to pstate of el1 when we return to it, so we set some configurations in it.
    // mask all exceptions except irq interrupts.
    // bit 9, D (debug exception mask) = 1
    // bit 8, A (serror interrupt mask) = 1
    // bit 7, I (irq interrupt mask) = 0
    // bit 6, F (fiq interrupt mask) = 1
    // we want to return to el1 with sp_el1.
    // bit 3-0, M (el1 with sp_el1 (h)) = 5
    mov x1, #0x345
    msr spsr_el2, x1
    // set address to return to from el2.
    adr x1, exit_to_el1
    msr elr_el2, x1
    eret
exit_to_el1:
    // set exception table for el1.
    ldr x1, =vector_table
    msr vbar_el1, x1
    // branch to main c++ function. x0 still contains core index, which is used as parameter for the main function.
    b main
    // infinite loop is here in case main accidentally returns.
hang:
    b hang

// stackpointers
.balign 8
stack_el2: .quad __stack_core_0_el2, __stack_core_1_el2, __stack_core_2_el2, __stack_core_3_el2
stack_el1: .quad __stack_core_0_el1, __stack_core_1_el1, __stack_core_2_el1, __stack_core_3_el1
stack_el0: .quad __stack_core_0_el0, __stack_core_1_el0, __stack_core_2_el0, __stack_core_3_el0

.macro vector handler
.balign 0x80
b \handler
.endm

.balign 0x800
.globl    vector_table
vector_table:
    // handlers for exceptions from current el1 with sp_el0
    // won't be called, since we run in el1 with sp_el1.
    vector syn_cur_el0 // synchronous
    vector irq_cur_el0 // irq
    vector fiq_cur_el0 // fiq
    vector err_cur_el0 // serror

    // handlers for exceptions from current el1 with sp_el1
    vector syn_cur_elx // synchronous
    vector irq_cur_elx_wrapper // irq
    vector fiq_cur_elx // fiq
    vector err_cur_elx // serror

    // handlers for exceptions from el0 (with el0 running aarch64)
    // won't be called, since we run in el1.
    vector syn_low64_elx // synchronous
    vector irq_low64_elx // irq
    vector fiq_low64_elx // fiq
    vector err_low64_elx // serror

    // handlers for exceptions from el0 (with el0 running aarch32)
    // won't be called, since we run in el1.
    vector syn_low32_elx // synchronous
    vector irq_low32_elx // irq
    vector fiq_low32_elx // fiq
    vector err_low32_elx // serror

// makes sure the interrupt handler correctly returns to regular code (without modifying the state of the registers).
irq_cur_elx_wrapper:
    // save x29 and x30 to stack.
    stp x29, x30, [sp, #-16]!

    // save elr_el1 and spsr_el1 to stack.
    mrs x29, elr_el1
    mrs x30, spsr_el1
    stp x29, x30, [sp, #-16]!

    // save x0 - x28 to stack.
    stp x27, x28, [sp, #-16]!
    stp x25, x26, [sp, #-16]!
    stp x23, x24, [sp, #-16]!
    stp x21, x22, [sp, #-16]!
    stp x19, x20, [sp, #-16]!
    stp x17, x18, [sp, #-16]!
    stp x15, x16, [sp, #-16]!
    stp x13, x14, [sp, #-16]!
    stp x11, x12, [sp, #-16]!
    stp x9, x10, [sp, #-16]!
    stp x7, x8, [sp, #-16]!
    stp x5, x6, [sp, #-16]!
    stp x3, x4, [sp, #-16]!
    stp x1, x2, [sp, #-16]!
    str x0, [sp, #-16]!

    // call actual handler.
    bl irq_cur_elx

    // restore x0 - x28 from stack.
    ldr x0, [sp], #16
    ldp x1, x2, [sp], #16
    ldp x3, x4, [sp], #16
    ldp x5, x6, [sp], #16
    ldp x7, x8, [sp], #16
    ldp x9, x10, [sp], #16
    ldp x11, x12, [sp], #16
    ldp x13, x14, [sp], #16
    ldp x15, x16, [sp], #16
    ldp x17, x18, [sp], #16
    ldp x19, x20, [sp], #16
    ldp x21, x22, [sp], #16
    ldp x23, x24, [sp], #16
    ldp x25, x26, [sp], #16
    ldp x27, x28, [sp], #16

    // restore elr_el1 and spsr_el1 from stack.
    ldp x29, x30, [sp], #16
    msr elr_el1, x29
    msr spsr_el1, x30

    // restore x29 and x30 from stack.
    ldp x29, x30, [sp], #16

    eret

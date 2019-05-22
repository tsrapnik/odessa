#include "buddy_heap.h"
#include "effect_chorus.h"
#include "effect_graph.h"
#include "memory.h"
#include "scratchpad.h"
#include "screen.h"
#include "string.h"
#include "uart.h"
#include "vc_gpu.h"
#include "vc_mailbox_framebuffer.h"
#include "vc_mailbox_property_tags.h"

//todo: remove and replace with uart logging.
screen* a_global_screen;
uart* a_uart;

// void delay()
// {
//     for(volatile u32 count = 0; count < 150; count++)
//         ;
// }

extern "C" i32 main(void)
{
    //clear bss.
    extern u8 __bss_start;
    extern u8 __bss_end;
    for(u8* pBSS = &__bss_start; pBSS < &__bss_end; pBSS++)
    {
        *pBSS = 0;
    }

    //call construtors of static objects.
    extern void (*__init_start)(void);
    extern void (*__init_end)(void);
    for(void (**constructor)(void) = &__init_start; constructor < &__init_end; constructor++)
    {
        (**constructor)();
    }

    //todo: change adressing to vc_gpu before mmu can be enabled.
    // memory::enable_mmu();

    buddy_heap::initialize();

    unsigned int max_clockrate = vc_mailbox_property_tags::get_max_clock_rate(vc_mailbox_property_tags::clock_id::arm);
    vc_mailbox_property_tags::set_clock_rate(vc_mailbox_property_tags::clock_id::arm, max_clockrate);

    vc_mailbox_framebuffer a_vc_mailbox_framebuffer;
    vc_gpu a_vc_gpu(a_vc_mailbox_framebuffer.get_buffer(), 800, 480);

    //todo: remove and replace with uart logging.
    // screen a_screen(vector_2_u32(800, 480), reinterpret_cast<color*>(a_vc_mailbox_framebuffer.get_buffer().to_arm_pointer()));
    // a_global_screen = &a_screen;
    // char a_string[20];
    // a_screen.draw_text(string::to_string(reinterpret_cast<usize>(&__heap_start), a_string), vector_2_u32(20, 20));
    // a_screen.draw_text(string::to_string(reinterpret_cast<usize>(heap_base), a_string), vector_2_u32(20, 120));
    // while(true)
    // {
    // }

    //////////////gpio

// #define ARM_IO_BASE 0x3F000000
// #define ARM_GPIO_BASE (ARM_IO_BASE + 0x200000)

// #define ARM_GPIO_GPFSEL0 (ARM_GPIO_BASE + 0x00)
// #define ARM_GPIO_GPPUD (ARM_GPIO_BASE + 0x94)
// #define ARM_GPIO_GPPUDCLK0 (ARM_GPIO_BASE + 0x98)

//     //32////
//     u64 nClkReg = ARM_GPIO_GPPUDCLK0 + (32 / 32) * 4;
//     u32 nShift = 32 % 32;

//     *(u32 volatile*)ARM_GPIO_GPPUD = 0;
//     // delay();
//     *(u32 volatile*)nClkReg = 1 << nShift;
//     // delay();
//     *(u32 volatile*)ARM_GPIO_GPPUD = 0;
//     *(u32 volatile*)nClkReg = 0;

//     u64 nSelReg = ARM_GPIO_GPFSEL0 + (32 / 10) * 4;
//     nShift = (32 % 10) * 3;

//     u32 nValue = *(u32 volatile*)nSelReg;
//     nValue &= ~(7 << nShift);
//     nValue |= 0 << nShift;
//     *(u32 volatile*)nSelReg = nValue;

//     ////33////
//     nClkReg = ARM_GPIO_GPPUDCLK0 + (33 / 32) * 4;
//     nShift = 33 % 32;

//     *(u32 volatile*)ARM_GPIO_GPPUD = 0;
//     // delay();
//     *(u32 volatile*)nClkReg = 1 << nShift;
//     // delay();
//     *(u32 volatile*)ARM_GPIO_GPPUD = 0;
//     *(u32 volatile*)nClkReg = 0;

//     nSelReg = ARM_GPIO_GPFSEL0 + (33 / 10) * 4;
//     nShift = (33 % 10) * 3;

//     nValue = *(u32 volatile*)nSelReg;
//     nValue &= ~(7 << nShift);
//     nValue |= 0 << nShift;
//     *(u32 volatile*)nSelReg = nValue;

//     ////14////
//     nClkReg = ARM_GPIO_GPPUDCLK0 + (14 / 32) * 4;
//     nShift = 14 % 32;

//     *(u32 volatile*)ARM_GPIO_GPPUD = 0;
//     // delay();
//     *(u32 volatile*)nClkReg = 1 << nShift;
//     // delay();
//     *(u32 volatile*)ARM_GPIO_GPPUD = 0;
//     *(u32 volatile*)nClkReg = 0;

//     nSelReg = ARM_GPIO_GPFSEL0 + (14 / 10) * 4;
//     nShift = (14 % 10) * 3;

//     nValue = *(u32 volatile*)nSelReg;
//     nValue &= ~(7 << nShift);
//     nValue |= 4 << nShift;
//     *(u32 volatile*)nSelReg = nValue;

//     u32 val14 = nValue;
//     u64 add14 = nSelReg;

//     ////15////
//     nClkReg = ARM_GPIO_GPPUDCLK0 + (15 / 32) * 4;
//     nShift = 15 % 32;

//     *(u32 volatile*)ARM_GPIO_GPPUD = 0;
//     // delay();
//     *(u32 volatile*)nClkReg = 1 << nShift;
//     // delay();
//     *(u32 volatile*)ARM_GPIO_GPPUD = 0;
//     *(u32 volatile*)nClkReg = 0;

//     nSelReg = ARM_GPIO_GPFSEL0 + (15 / 10) * 4;
//     nShift = (15 % 10) * 3;

//     nValue = *(u32 volatile*)nSelReg;
//     nValue &= ~(7 << nShift);
//     nValue |= 4 << nShift;
//     *(u32 volatile*)nSelReg = nValue;

//     u32 val15 = nValue;
//     u64 add15 = nSelReg;
    //////////////

    a_uart = uart::create(uart::device::uart0);
    a_uart->write("\r\n", 2);
    a_uart->write("\r\n", 2);
    a_uart->write("all done bitches1.\r\n", 19);
    a_uart->write("\r\n", 2);
    // a_uart->write(string::to_string(add14), 19);
    // a_uart->write("\r\n", 2);
    // a_uart->write(string::to_string(*(u32 volatile*)add14), 19);
    // a_uart->write("\r\n", 2);
    // a_uart->write(string::to_string(val14), 19);
    // a_uart->write("\r\n", 2);
    // a_uart->write(string::to_string(add15), 19);
    // a_uart->write("\r\n", 2);
    // a_uart->write(string::to_string(*(u32 volatile*)add15), 19);
    // a_uart->write("\r\n", 2);
    // a_uart->write(string::to_string(val15), 19);
    // a_uart->write("\r\n", 2);
    // a_uart->write("\r\n", 2);
    // a_uart->write("\r\n", 2);

    gpio* gpio_32 = gpio::create(gpio::device::gpio_32,
                                 gpio::pull_up_down_state::disable_pull_up_or_down,
                                 gpio::function::input);
    gpio* gpio_33 = gpio::create(gpio::device::gpio_33,
                                 gpio::pull_up_down_state::disable_pull_up_or_down,
                                 gpio::function::input);
    gpio* tx_pin = gpio::create(gpio::device::gpio_14,
                                gpio::pull_up_down_state::disable_pull_up_or_down,
                                gpio::function::alternate_function_0);
    gpio* rx_pin = gpio::create(gpio::device::gpio_15,
                                gpio::pull_up_down_state::disable_pull_up_or_down,
                                gpio::function::alternate_function_0);

    
    a_uart->write("\r\n", 2);
    a_uart->write("\r\n", 2);
    a_uart->write("all done bitches2.\r\n", 19);
    a_uart->write("\r\n", 2);
    while(true)
        ;

    u16 offset = 0;
    u32 vertices_size = 4;
    vc_gpu::vertex vertices[vertices_size];

    vertices[0].xs = 200 << 4;
    vertices[0].ys = 100 << 4;
    vertices[0].zs = 1.0f;
    vertices[0].wc = 1.0f;
    vertices[0].r = 1.0f;
    vertices[0].g = 0.0f;
    vertices[0].b = 0.0f;

    vertices[1].xs = 600 << 4;
    vertices[1].ys = 100 << 4;
    vertices[1].zs = 1.0f;
    vertices[1].wc = 1.0f;
    vertices[1].r = 0.0f;
    vertices[1].g = 1.0f;
    vertices[1].b = 0.0f;

    vertices[2].xs = offset << 4;
    vertices[2].ys = 300 << 4;
    vertices[2].zs = 1.0f;
    vertices[2].wc = 1.0f;
    vertices[2].r = 0.0f;
    vertices[2].g = 0.0f;
    vertices[2].b = 1.0f;

    vertices[3].xs = 600 << 4;
    vertices[3].ys = 300 << 4;
    vertices[3].zs = 1.0f;
    vertices[3].wc = 1.0f;
    vertices[3].r = 1.0f;
    vertices[3].g = 1.0f;
    vertices[3].b = 1.0f;

    u32 triangles_size = 2;
    vc_gpu::triangle triangles[triangles_size];

    triangles[0].index_0 = 0;
    triangles[0].index_1 = 1;
    triangles[0].index_2 = 2;

    triangles[1].index_0 = 3;
    triangles[1].index_1 = 2;
    triangles[1].index_2 = 1;

    list<vc_gpu::triangle> triangles_list;
    for(u32 index = 0; index < triangles_size; index++)
        triangles_list.append_copy(triangles[index]);

    list<vc_gpu::vertex> vertices_list;
    for(u32 index = 0; index < vertices_size; index++)
        vertices_list.append_copy(vertices[index]);

    color background_color(255, 0, 0, 255);
    bool correct = true;
    if(!((triangles_list.get_size() == triangles_size) && (vertices_list.get_size() == vertices_size)))
        correct = false;

    u32 index = 0;
    list_iterator<vc_gpu::triangle> triangles_iterator(triangles_list);
    for(triangles_iterator.to_first(); !triangles_iterator.at_end(); triangles_iterator++)
    {
        vc_gpu::triangle& current_triangle = triangles_iterator.get_data_reference();
        if(triangles[index].index_0 != current_triangle.index_0 ||
           triangles[index].index_1 != current_triangle.index_1 ||
           triangles[index].index_2 != current_triangle.index_2)
            correct = false;

        index++;
    }

    index = 0;
    list_iterator<vc_gpu::vertex> vertices_iterator(vertices_list);
    for(vertices_iterator.to_first(); !vertices_iterator.at_end(); vertices_iterator++)
    {
        vc_gpu::vertex& current_vertex = vertices_iterator.get_data_reference();
        if(vertices[index].xs == current_vertex.xs ||
           vertices[index].ys == current_vertex.ys ||
           vertices[index].zs == current_vertex.zs ||
           vertices[index].wc == current_vertex.wc ||
           vertices[index].r == current_vertex.r ||
           vertices[index].g == current_vertex.g ||
           vertices[index].b == current_vertex.b)
            correct = false;

        index++;
    }

    if(correct)
        background_color = color(0, 255, 0, 255);

    while(1)
    {
        a_vc_gpu.set_triangles(vertices, vertices_size, triangles, triangles_size, background_color);
        // a_vc_gpu.set_triangles(vertices_list, triangles_list, background_color);
        a_vc_gpu.render();

        offset++;
        if(offset >= 800)
            offset = 0;
        vertices[2].xs = offset << 4;
    }
    return (0);
}

extern "C" void syn_cur_el0()
{
    while(true)
        a_global_screen->draw_text("syn_cur_el0", vector_2_u32(0, 0));
}
extern "C" void irq_cur_el0()
{
    while(true)
        a_global_screen->draw_text("irq_cur_el0", vector_2_u32(0, 0));
}
extern "C" void fiq_cur_el0()
{
    while(true)
        a_global_screen->draw_text("fiq_cur_el0", vector_2_u32(0, 0));
}
extern "C" void err_cur_el0()
{
    while(true)
        a_global_screen->draw_text("err_cur_el0", vector_2_u32(0, 0));
}

extern "C" void syn_cur_elx()
{
    while(true)
        a_global_screen->draw_text("syn_cur_elx", vector_2_u32(0, 0));
}
extern "C" void irq_cur_elx()
{
    while(true)
        a_global_screen->draw_text("irq_cur_elx", vector_2_u32(0, 0));
}
extern "C" void fiq_cur_elx()
{
    while(true)
        a_global_screen->draw_text("fiq_cur_elx", vector_2_u32(0, 0));
}
extern "C" void err_cur_elx()
{
    while(true)
        a_global_screen->draw_text("err_cur_elx", vector_2_u32(0, 0));
}

extern "C" void syn_low64_elx()
{
    while(true)
        a_global_screen->draw_text("csyn_low64_elx", vector_2_u32(0, 0));
}
extern "C" void irq_low64_elx()
{
    while(true)
        a_global_screen->draw_text("irq_low64_elx", vector_2_u32(0, 0));
}
extern "C" void fiq_low64_elx()
{
    while(true)
        a_global_screen->draw_text("fiq_low64_elx", vector_2_u32(0, 0));
}
extern "C" void err_low64_elx()
{
    while(true)
        a_global_screen->draw_text("err_low64_elx", vector_2_u32(0, 0));
}

extern "C" void syn_low32_elx()
{
    while(true)
        a_global_screen->draw_text("syn_low32_elx", vector_2_u32(0, 0));
}
extern "C" void irq_low32_elx()
{
    while(true)
        a_global_screen->draw_text("irq_low32_elx", vector_2_u32(0, 0));
}
extern "C" void fiq_low32_elx()
{
    while(true)
        a_global_screen->draw_text("fiq_low32_elx", vector_2_u32(0, 0));
}
extern "C" void err_low32_elx()
{
    while(true)
        a_global_screen->draw_text("err_low32_elx", vector_2_u32(0, 0));
}

//dummy functions to avoid linker complaints.
extern "C" void __cxa_pure_virtual()
{
    while(1)
        ;
}

extern "C" void __aeabi_atexit(void)
{
    while(1)
        ;
}

extern "C" void __dso_handle(void)
{
    while(1)
        ;
}

extern "C" void __cxa_guard_acquire(void)
{
}

extern "C" void __cxa_guard_release(void)
{
}

extern "C" void __cxa_atexit(void)
{
}

extern "C" void _exit()
{
}

extern "C" void _kill()
{
}

extern "C" void _getpid()
{
}

extern "C" void __gxx_personality_v0()
{
}

extern "C" char* __attribute__((weak)) _sbrk(int incr)
{
    return (char*)0;
}

// extern "C" void memcpy(void* destination, void* source, usize size)
// {
// u32 size_u64 = size / 8;
// u32 size_u8 = size % 8;

// u64* destination_u64 = reinterpret_cast<u64*>(destination);
// u64* source_u64 = reinterpret_cast<u64*>(source);
// for(u32 i = 0; i < size_u64; i++)
// {
//     *destination_u64 = *source_u64;
//     destination_u64++;
//     source_u64++;
// }

// u8* destination_u8 = reinterpret_cast<u8*>(destination_u64);
// u8* source_u8 = reinterpret_cast<u8*>(source_u64);
// for(u32 i = 0; i < size_u8; i++)
// {
//      *destination_u8 = *source_u8;
//      destination_u8++;
//      source_u8++;
// }
// u8* destination_u8 = reinterpret_cast<u8*>(destination);
// u8* source_u8 = reinterpret_cast<u8*>(source);
// for(u32 i = 0; i < size; i++)
// {
//      *destination_u8 = *source_u8;
//      destination_u8++;
//      source_u8++;
// }
// }

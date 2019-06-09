#include "buddy_heap.h"
#include "effect_chorus.h"
#include "effect_graph.h"
#include "memory.h"
#include "scratchpad.h"
#include "string.h"
#include "uart.h"
#include "vc_gpu.h"
#include "vc_mailbox_framebuffer.h"
#include "vc_mailbox_property_tags.h"
#include "math.h"

//todo: make uart singleton like device.
uart* a_uart;

void initialize_cpp_runtime()
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
}

extern "C" i32 main(void)
{
    initialize_cpp_runtime();

    //todo: change adressing to vc_gpu before mmu can be enabled.
    // memory::enable_mmu();

    buddy_heap::initialize();

    unsigned int max_clockrate = vc_mailbox_property_tags::get_max_clock_rate(vc_mailbox_property_tags::clock_id::arm);
    vc_mailbox_property_tags::set_clock_rate(vc_mailbox_property_tags::clock_id::arm, max_clockrate);

    vc_mailbox_framebuffer a_vc_mailbox_framebuffer;
    vc_gpu a_vc_gpu(a_vc_mailbox_framebuffer.get_buffer(), 800, 480);

    a_uart = uart::create(uart::device::uart0);
    a_uart->write("uart created.\r\n");
    a_uart->write("\r\n");

    a_uart->write(string::to_string(math::leading_zeroes(0x00fffffffffffffful)), 18);
    a_uart->write("\r\n");
    a_uart->write(string::to_string(math::leading_zeroes(0x00ffffffu)), 18);
    a_uart->write("\r\n");
    a_uart->write(string::to_string(static_cast<u64>(math::sqrt(16.0f))), 18);
    a_uart->write("\r\n");
    a_uart->write(string::to_string(static_cast<u64>(math::sqrt(16.0))), 18);
    a_uart->write("\r\n");

    scene_2d a_scene;
    a_scene.append_triangle(vector_2_f32(100.0f, 100.0f),
                            vector_2_f32(400.0f, 100.0f),
                            vector_2_f32(100.0f, 400.0f),
                            color(255, 255, 255, 255));
    a_scene.append_line(vector_2_f32(100.0f, 100.0f),
                        vector_2_f32(500.0f, 20.0f),
                        color(0, 255, 255, 255),
                        1.0f);

    while(true)
    {
        a_vc_gpu.set_triangles(a_scene, color(0, 0, 0, 255));
        a_vc_gpu.render();

        a_scene.vertices.get_reference_first().a_color.red++;
        a_scene.vertices.get_reference_first().position.coordinate[0]++;
    }
    return (0);
}

extern "C" void syn_cur_el0()
{
    while(true)
        a_uart->write("syn_cur_el0\r\n");
}
extern "C" void irq_cur_el0()
{
    while(true)
        a_uart->write("irq_cur_el0\r\n");
}
extern "C" void fiq_cur_el0()
{
    while(true)
        a_uart->write("fiq_cur_el0\r\n");
}
extern "C" void err_cur_el0()
{
    while(true)
        a_uart->write("err_cur_el0\r\n");
}

extern "C" void syn_cur_elx()
{
    while(true)
        a_uart->write("syn_cur_elx\r\n");
}
extern "C" void irq_cur_elx()
{
    while(true)
        a_uart->write("irq_cur_elx\r\n");
}
extern "C" void fiq_cur_elx()
{
    while(true)
        a_uart->write("fiq_cur_elx\r\n");
}
extern "C" void err_cur_elx()
{
    while(true)
        a_uart->write("err_cur_elx\r\n");
}

extern "C" void syn_low64_elx()
{
    while(true)
        a_uart->write("csyn_low64_elx\r\n");
}
extern "C" void irq_low64_elx()
{
    while(true)
        a_uart->write("irq_low64_elx\r\n");
}
extern "C" void fiq_low64_elx()
{
    while(true)
        a_uart->write("fiq_low64_elx\r\n");
}
extern "C" void err_low64_elx()
{
    while(true)
        a_uart->write("err_low64_elx\r\n");
}

extern "C" void syn_low32_elx()
{
    while(true)
        a_uart->write("syn_low32_elx\r\n");
}
extern "C" void irq_low32_elx()
{
    while(true)
        a_uart->write("irq_low32_elx\r\n");
}
extern "C" void fiq_low32_elx()
{
    while(true)
        a_uart->write("fiq_low32_elx\r\n");
}
extern "C" void err_low32_elx()
{
    while(true)
        a_uart->write("err_low32_elx\r\n");
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

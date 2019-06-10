#include "buddy_heap.h"
#include "effect_chorus.h"
#include "effect_graph.h"
#include "math.h"
#include "memory.h"
#include "scratchpad.h"
#include "string.h"
#include "uart.h"
#include "vc_gpu.h"
#include "vc_mailbox_framebuffer.h"
#include "vc_mailbox_property_tags.h"

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

    //should be created as soon as possible to enable debugging.
    a_uart = uart::create(uart::device::uart0);
    a_uart->write("uart created.\r\n");

    unsigned int max_clockrate = vc_mailbox_property_tags::get_max_clock_rate(vc_mailbox_property_tags::clock_id::arm);
    vc_mailbox_property_tags::set_clock_rate(vc_mailbox_property_tags::clock_id::arm, max_clockrate);

    vc_mailbox_framebuffer a_vc_mailbox_framebuffer;
    vc_gpu a_vc_gpu(a_vc_mailbox_framebuffer.get_buffer(), 800, 480);

    scene_2d a_scene;

    effect_graph a_effect_graph;
    effect_chorus a_effect_chorus(rectangle(vector_2_f32(100.0f, 100.0f), vector_2_f32(200.0f, 100.0f)),
                                  color(255, 100, 0, 255));

    a_effect_graph.add_effect(&a_effect_chorus);

    while(true)
    {
        //todo: to string is memory leak.
        a_scene.clear();
        a_effect_graph.draw(a_scene);
        a_vc_gpu.set_triangles(a_scene, color(100, 0, 100, 255));
        a_vc_gpu.render();
        buddy_heap::print();

        a_scene.vertices.get_reference_first().a_color.red++;
        a_scene.vertices.get_reference_first().position.coordinate[0]++;
    }
    return (0);
}

extern "C" void syn_cur_el0()
{
    a_uart->write("syn_cur_el0\r\n");
    while(true)
        ;
}
extern "C" void irq_cur_el0()
{
    a_uart->write("irq_cur_el0\r\n");
    while(true)
        ;
}
extern "C" void fiq_cur_el0()
{
    a_uart->write("fiq_cur_el0\r\n");
    while(true)
        ;
}
extern "C" void err_cur_el0()
{
    a_uart->write("err_cur_el0\r\n");
    while(true)
        ;
}

extern "C" void syn_cur_elx()
{
    a_uart->write("syn_cur_elx\r\n");
    while(true)
        ;
}
extern "C" void irq_cur_elx()
{
    a_uart->write("irq_cur_elx\r\n");
    while(true)
        ;
}
extern "C" void fiq_cur_elx()
{
    a_uart->write("fiq_cur_elx\r\n");
    while(true)
        ;
}
extern "C" void err_cur_elx()
{
    a_uart->write("err_cur_elx\r\n");
    while(true)
        ;
}

extern "C" void syn_low64_elx()
{
    a_uart->write("csyn_low64_elx\r\n");
    while(true)
        ;
}
extern "C" void irq_low64_elx()
{
    a_uart->write("irq_low64_elx\r\n");
    while(true)
        ;
}
extern "C" void fiq_low64_elx()
{
    a_uart->write("fiq_low64_elx\r\n");
    while(true)
        ;
}
extern "C" void err_low64_elx()
{
    a_uart->write("err_low64_elx\r\n");
    while(true)
        ;
}

extern "C" void syn_low32_elx()
{
    a_uart->write("syn_low32_elx\r\n");
    while(true)
        ;
}
extern "C" void irq_low32_elx()
{
    a_uart->write("irq_low32_elx\r\n");
    while(true)
        ;
}
extern "C" void fiq_low32_elx()
{
    a_uart->write("fiq_low32_elx\r\n");
    while(true)
        ;
}
extern "C" void err_low32_elx()
{
    a_uart->write("err_low32_elx\r\n");
    while(true)
        ;
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

extern "C" void memcpy(void* destination, void* source, usize size)
{
    u32 size_u64 = size / 8;
    u32 size_u8 = size % 8;

    u64* destination_u64 = reinterpret_cast<u64*>(destination);
    u64* source_u64 = reinterpret_cast<u64*>(source);
    for(u32 i = 0; i < size_u64; i++)
    {
        *destination_u64 = *source_u64;
        destination_u64++;
        source_u64++;
    }

    u8* destination_u8 = reinterpret_cast<u8*>(destination_u64);
    u8* source_u8 = reinterpret_cast<u8*>(source_u64);
    for(u32 i = 0; i < size_u8; i++)
    {
        *destination_u8 = *source_u8;
        destination_u8++;
        source_u8++;
    }
}

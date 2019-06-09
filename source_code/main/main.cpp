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
    a_uart->write("\r\n");
    a_uart->write("uart created.\r\n");
    a_uart->write("\r\n");

    u16 offset = 0;
    u32 vertices_size = 4;
    vc_gpu::vertex vertices[vertices_size];

    vertices[0].xs = 200 << 4;
    vertices[0].ys = 100 << 4;
    vertices[0].zs = 1.0f;
    vertices[0].wc = 1.0f;
    vertices[0].r = 0.0f;
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

    list_iterator<vc_gpu::vertex> vertices_iterator(vertices_list);
    vertices_iterator.to_first();
    vertices_iterator++;
    vertices_iterator++;
    vc_gpu::vertex& moving_vertex = vertices_iterator.get_data_reference();

    scene_2d a_scene;
    a_scene.append_triangle(vector_2_f32(100.0f, 100.0f),
                            vector_2_f32(400.0f, 100.0f),
                            vector_2_f32(100.0f, 400.0f),
                            color(255, 255, 255, 255));
    a_scene.append_line(vector_2_f32(100.0f, 480.0f),
                        vector_2_f32(500.0f, 20.0f),
                        color(0, 255, 255, 255),
                        5.0f);

    while(1)
    {
        // a_vc_gpu.set_triangles(vertices_list, triangles_list, color(0, 0, 0, 255));
        a_vc_gpu.set_triangles(a_scene, color(0, 0, 0, 255));
        a_vc_gpu.render();

        offset++;
        if(offset >= 900)
            offset = 0;
        moving_vertex.xs = offset << 4;
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

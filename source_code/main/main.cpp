#include "buddy_heap.h"
#include "effect_chorus.h"
#include "effect_graph.h"
#include "gpu.h"
#include "mailbox_framebuffer.h"
#include "mailbox_property_tags.h"
#include "memory.h"
#include "scratchpad.h"
#include "screen.h"
#include "string.h"

//todo: all mailbox and hardware accessing classes should use struct of volatile enum.
//todo: figure out why use of d0 registers lets gpu fail and seperate memory assignment for triangle indices gives strange black corner artifact.

screen* a_global_screen;

extern "C" i32 main(void)
{
    //clear bss.
    // extern u8 __bss_start;
    // extern u8 __bss_end;
    // for(u8* pBSS = &__bss_start; pBSS < &__bss_end; pBSS++)
    // {
    //     *pBSS = 0;
    // }

    // //call construtors of static objects.
    // extern void (*__init_start)(void);
    // extern void (*__init_end)(void);
    // for(void (**constructor)(void) = &__init_start; constructor < &__init_end; constructor++)
    // {
    //     (**constructor)();
    // }

    mailbox_property_tags& a_mailbox_property_tags = mailbox_property_tags::get_handle();
    u32 max_clock_rate = a_mailbox_property_tags.get_max_clock_rate(mailbox_property_tags::clock_id::arm);
    a_mailbox_property_tags.set_clock_rate(mailbox_property_tags::clock_id::arm, max_clock_rate);

    // memory::enable_mmu();
    buddy_heap::initialize();
    mailbox_framebuffer a_mailbox_framebuffer;
    color* framebuffer = a_mailbox_framebuffer.get_framebuffer();
    screen a_screen(vector_2_u32(800, 480), framebuffer);

    a_global_screen = &a_screen;

    effect_graph a_effect_graph(&a_screen);
    effect_chorus a_effect_chorus(rectangle(vector_2_u32(50, 50), vector_2_u32(100, 50)), color(127, 0, 0, 255));
    a_effect_graph.add_effect(&a_effect_chorus);

    u32 vertex_buffer_size = 3;
    gpu::vertex vertex_buffer[vertex_buffer_size];
    vertex_buffer[0].x = (0) << 4;
    vertex_buffer[0].y = (0) << 4;
    vertex_buffer[0].z = 1.0;
    vertex_buffer[0].w = 1.0;
    vertex_buffer[0].r = 1.0;
    vertex_buffer[0].g = 1.0;
    vertex_buffer[0].b = 1.0;

    vertex_buffer[1].x = (00) << 4;
    vertex_buffer[1].y = (0) << 4;
    vertex_buffer[1].z = 1.0;
    vertex_buffer[1].w = 1.0;
    vertex_buffer[1].r = 1.0;
    vertex_buffer[1].g = 1.0;
    vertex_buffer[1].b = 1.0;

    vertex_buffer[2].x = (0) << 4;
    vertex_buffer[2].y = (0) << 4;
    vertex_buffer[2].z = 1.0;
    vertex_buffer[2].w = 1.0;
    vertex_buffer[2].r = 1.0;
    vertex_buffer[2].g = 1.0;
    vertex_buffer[2].b = 1.0;

    u32 triangle_buffer_size = 1;
    gpu::triangle triangle_buffer[triangle_buffer_size];
    triangle_buffer[0].index_0 = 0;
    triangle_buffer[0].index_1 = 1;
    triangle_buffer[0].index_2 = 2;

    gpu a_gpu(vertex_buffer_size, triangle_buffer_size);
    a_gpu.a_screen = &a_screen;
    u32 frame_buffer_handle = static_cast<u32>(reinterpret_cast<u64>(a_mailbox_framebuffer.get_framebuffer()));
    a_gpu.render(800, 480, frame_buffer_handle, vertex_buffer, triangle_buffer, vertex_buffer_size, triangle_buffer_size);

    u32 i = 0;
    while(true)
    {
        // a_gpu.testTriangle(800,480,frame_buffer_handle,i);
        a_screen.draw_text("x", vector_2_u32(i, 0));
        i += 60;
        a_gpu.render(800, 480, frame_buffer_handle, vertex_buffer, triangle_buffer, vertex_buffer_size, triangle_buffer_size);
    }
    return 0;
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
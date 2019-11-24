#include "buddy_heap.h"
#include "effect_chorus.h"
#include "effect_delay.h"
#include "effect_graph.h"
#include "effect_looper.h"
#include "effect_sink.h"
#include "effect_source.h"
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
    effect_delay a_effect_delay(rectangle(vector_2_f32(100.0f, 100.0f), vector_2_f32(200.0f, 100.0f)),
                                color(0, 100, 200, 125));
    effect_looper a_effect_looper(rectangle(vector_2_f32(500.0f, 100.0f), vector_2_f32(200.0f, 100.0f)),
                                  color(100, 100, 200, 125));
    i32 sink, source;
    effect_sink a_effect_sink(&sink,
                              rectangle(vector_2_f32(100.0f, 300.0f), vector_2_f32(200.0f, 100.0f)),
                              color(200, 100, 20, 125));
    effect_source a_effect_source(&source,
                                  rectangle(vector_2_f32(500.0f, 300.0f), vector_2_f32(200.0f, 100.0f)),
                                  color(50, 80, 50, 125));

    a_effect_graph.add_effect(&a_effect_chorus);
    a_effect_graph.add_effect(&a_effect_delay);
    a_effect_graph.add_effect(&a_effect_looper);
    a_effect_graph.add_effect(&a_effect_sink);
    a_effect_graph.add_effect(&a_effect_source);

    constexpr f32 speed = 0.2f;
    f32 dx = speed;
    f32 dy = speed;

    u8 old_points_size = 0;
    u32 old_x_position = 0;
    u32 old_y_position = 0;
    volatile vc_mailbox_property_tags::touch_buffer a_touch_buffer;
    vc_mailbox_property_tags::set_touch_buffer(const_cast<vc_mailbox_property_tags::touch_buffer*>(&a_touch_buffer));

    while(true)
    {
        //todo: to string is memory leak.
        a_scene.clear();
        a_effect_graph.draw(a_scene);
        a_vc_gpu.set_triangles(a_scene, color(100, 0, 100, 255));
        a_vc_gpu.render();

        a_effect_chorus.move(vector_2_f32(dx, dy));
        if(a_effect_chorus.get_bounding_box().get_center().coordinate[0] > 800.0f)
        {
            dx = -speed;
            a_uart->write("hit right.\r\n");
        }
        if(a_effect_chorus.get_bounding_box().get_center().coordinate[0] < 0.0f)
        {
            dx = speed;
            a_uart->write("hit left.\r\n");
        }
        if(a_effect_chorus.get_bounding_box().get_center().coordinate[1] > 480.0f)
        {
            dy = -speed;
            a_uart->write("hit bottom.\r\n");
        }
        if(a_effect_chorus.get_bounding_box().get_center().coordinate[1] < 0.0f)
        {
            dy = speed;
            a_uart->write("hit top.\r\n");
        }

        u8 new_points_size = a_touch_buffer.points_size;
        if(new_points_size > 10)
            new_points_size = old_points_size;
        u32 new_x_position = ((a_touch_buffer.points[0].x_high_word & 0xf) << 8) | a_touch_buffer.points[0].x_low_word;
        if(new_x_position > 799)
            new_x_position = old_x_position;
        u32 new_y_position = ((a_touch_buffer.points[0].y_high_word & 0xf) << 8) | a_touch_buffer.points[0].y_low_word;
        if(new_y_position > 479)
            new_y_position = old_y_position;
        if((new_points_size != old_points_size) ||
           (new_x_position != old_x_position) ||
           (new_y_position != old_y_position))
        {
            char buffer[19];
            a_uart->write("points: ");
            a_uart->write(string::to_string(new_points_size, buffer));
            a_uart->write(", ");
            a_uart->write(string::to_string(new_x_position, buffer));
            a_uart->write(", ");
            a_uart->write(string::to_string(new_y_position, buffer));
            a_uart->write(", ");
            a_uart->write(string::to_string(a_touch_buffer.points[0].reserved[0], buffer));
            a_uart->write(", ");
            a_uart->write(string::to_string(a_touch_buffer.points[0].reserved[1], buffer));
            a_uart->write(", ");
            a_uart->write(string::to_string(a_touch_buffer.device_mode, buffer));
            a_uart->write(", ");
            a_uart->write(string::to_string(a_touch_buffer.gesture_id, buffer));
            a_uart->write(".\r\n");

            old_points_size = new_points_size;
            old_x_position = new_x_position;
            old_y_position = new_y_position;
        }
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
    usize size_u64 = size / 8;
    usize size_u8 = size % 8;

    u64* destination_u64 = reinterpret_cast<u64*>(destination);
    u64* source_u64 = reinterpret_cast<u64*>(source);
    for(usize index = 0; index < size_u64; index++)
    {
        *destination_u64 = *source_u64;
        destination_u64++;
        source_u64++;
    }

    u8* destination_u8 = reinterpret_cast<u8*>(destination_u64);
    u8* source_u8 = reinterpret_cast<u8*>(source_u64);
    for(usize index = 0; index < size_u8; index++)
    {
        *destination_u8 = *source_u8;
        destination_u8++;
        source_u8++;
    }
}
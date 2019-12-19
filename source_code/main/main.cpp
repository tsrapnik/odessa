#include "assert.h"
#include "buddy_heap.h"
#include "effect_chorus.h"
#include "effect_delay.h"
#include "effect_graph.h"
#include "effect_looper.h"
#include "effect_sink.h"
#include "effect_source.h"
#include "gpio.h"
#include "i2s.h"
#include "math.h"
#include "memory.h"
#include "scratchpad.h"
#include "spi.h"
#include "string_.h"
#include "uart.h"
#include "vc_gpu.h"
#include "vc_mailbox_framebuffer.h"
#include "vc_mailbox_property_tags.h"

//todo: check for every created device if it does not return a nullptr.

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
    a_uart = uart::create(uart::device::uart_pl011);
    a_uart->write("uart created.\r\n");

    //todo: max clockrate appears random.
    u32 max_clockrate = vc_mailbox_property_tags::get_max_clock_rate(vc_mailbox_property_tags::clock_id::arm);
    vc_mailbox_property_tags::set_clock_rate(vc_mailbox_property_tags::clock_id::arm, max_clockrate);
    a_uart->write("clock speed is ");
    char buffer[19];
    a_uart->write(string_::to_string(max_clockrate, buffer));
    a_uart->write(" Hz.\r\n");

    //initialize gpu and draw a example scene to the screen.
    vc_mailbox_framebuffer a_vc_mailbox_framebuffer;
    vc_gpu a_vc_gpu(a_vc_mailbox_framebuffer.get_buffer(), 800, 480);

    scene_2d a_scene;

    effect_graph a_effect_graph;
    effect_chorus a_effect_chorus(rectangle(vector_2_f32(300.0f, 200.0f), vector_2_f32(200.0f, 100.0f)),
                                  color(255, 100, 0, 255));
    i32 sink, source;
    effect_sink a_effect_sink(&sink,
                              rectangle(vector_2_f32(550.0f, 200.0f), vector_2_f32(200.0f, 100.0f)),
                              color(200, 100, 20, 125));
    effect_source a_effect_source(&source,
                                  rectangle(vector_2_f32(50.0f, 200.0f), vector_2_f32(200.0f, 100.0f)),
                                  color(50, 80, 50, 125));

    a_effect_graph.add_effect(&a_effect_chorus);
    a_effect_graph.add_effect(&a_effect_sink);
    a_effect_graph.add_effect(&a_effect_source);

    //initialize touch screen.
    volatile vc_mailbox_property_tags::touch_buffer a_touch_buffer;
    vc_mailbox_property_tags::set_touch_buffer(const_cast<vc_mailbox_property_tags::touch_buffer*>(&a_touch_buffer));

    //initialize spi.
    spi* spi0 = spi::create(spi::device::spi0);
    assert(spi0 != nullptr);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wbitfield-enum-conversion"
    struct pga2500_settings
    {
        //bytes are swapped, to match little endian layout.
        bool gpo1_high : 1;
        bool gpo2_high : 1;
        bool gpo3_high : 1;
        bool gpo4_high : 1;
        enum class overload_indicator_enum
        {
            assert_5_1_v_rms = 0,
            assert_4_0_v_rms = 1,
        } overload_indicator : 1;
        w8 reserved_13 : 1;
        bool common_mode_servo_enabled : 1;
        bool dc_servo_enabled : 1;

        u8 gain : 6;
        w8 reserved_6_7 : 2;
    } __attribute__((packed));
    static_assert(sizeof(pga2500_settings) == 2, "pga 2500 settings size does not match datasheet.");
#pragma GCC diagnostic pop

    pga2500_settings preamp_settings = {};
    preamp_settings.gain = 40 - 9; //(0 is 0 dB, 1 to 56 is 10 to 65 dB, 57 to 63 is 65 dB).

    spi0->write(spi::cs_enum::cs0, reinterpret_cast<w8*>(&preamp_settings), sizeof(preamp_settings));
    spi0->write(spi::cs_enum::cs1, reinterpret_cast<w8*>(&preamp_settings), sizeof(preamp_settings));

    //initialize i2s.
    gpio* enable_adc = gpio::create(gpio::device::gpio_5,
                                    gpio::pull_up_down_state::disable_pull_up_or_down,
                                    gpio::function::output);
    assert(enable_adc != nullptr);

    gpio* enable_dac = gpio::create(gpio::device::gpio_6,
                                    gpio::pull_up_down_state::disable_pull_up_or_down,
                                    gpio::function::output);
    assert(enable_adc != nullptr);

    enable_adc->set_output(true);
    enable_dac->set_output(true);

    i2s* i2s0 = i2s::create(i2s::device::i2s0);
    assert(i2s0 != nullptr);

    vector_2_f32 old_mouse_position = vector_2_f32(0.0f, 0.0f);
    effect* selected_effect = nullptr;
    output* selected_output = nullptr;
    bool touching = false;

    while(true)
    {
        a_scene.clear();
        a_effect_graph.draw(a_scene);

        //todo: provide proper implementation, this is just for fooling around.
        if(!touching && a_touch_buffer.points_size == 1)
        {
            a_uart->write("touched.\r\n");
            //touched.
            vector_2_f32 mouse_position = vector_2_f32(
                static_cast<f32>(((a_touch_buffer.points[0].x_high_word & 0xf) << 8) | a_touch_buffer.points[0].x_low_word),
                static_cast<f32>(((a_touch_buffer.points[0].y_high_word & 0xf) << 8) | a_touch_buffer.points[0].y_low_word));

            effect* temp_selected_effect = a_effect_graph.get_selected_effect(mouse_position);
            if(temp_selected_effect != nullptr)
            {
                output* temp_selected_output = temp_selected_effect->get_selected_output(mouse_position);
                if(temp_selected_output != nullptr)
                {
                    selected_effect = nullptr;
                    selected_output = temp_selected_output;
                }
                else
                {
                    selected_effect = temp_selected_effect;
                    selected_output = nullptr;
                }
            }
            old_mouse_position = mouse_position;
            touching = true;
        }
        else if(touching && a_touch_buffer.points_size == 1)
        {
            //dragged.
            vector_2_f32 mouse_position = vector_2_f32(
                static_cast<f32>(((a_touch_buffer.points[0].x_high_word & 0xf) << 8) | a_touch_buffer.points[0].x_low_word),
                static_cast<f32>(((a_touch_buffer.points[0].y_high_word & 0xf) << 8) | a_touch_buffer.points[0].y_low_word));

            if(selected_effect != nullptr)
            {
                selected_effect->move(vector_2_f32::difference(mouse_position, old_mouse_position));
            }
            else if(selected_output != nullptr)
            {
                selected_output->draw_connection(a_scene, mouse_position);
            }
            old_mouse_position = mouse_position;
        }
        else if(touching && (a_touch_buffer.points_size == 0))
        {
            a_uart->write("released.\r\n");
            //released.
            if(selected_output != nullptr)
            {
                effect* selected_effect = a_effect_graph.get_selected_effect(old_mouse_position);
                if(selected_effect != nullptr)
                {
                    input* selected_input = selected_effect->get_selected_input(old_mouse_position);
                    if(selected_input != nullptr)
                    {
                        selected_input->connect_output(selected_output);
                    }
                }
            }
            selected_effect = nullptr;
            selected_output = nullptr;
            touching = false;
        }

        // todo: to string is memory leak.
        a_vc_gpu.set_triangles(a_scene, color(100, 0, 100, 255));
        a_vc_gpu.render();
    }
    return (0);
}

//exception handlers.
extern "C" void syn_cur_el0()
{
    a_uart->write("syn_cur_el0\r\n");
    while(true)
        ;
}
extern "C" void syn_cur_elx()
{
    a_uart->write("syn_cur_elx\r\n");
    while(true)
        ;
}
extern "C" void syn_low64_elx()
{
    a_uart->write("csyn_low64_elx\r\n");
    while(true)
        ;
}
extern "C" void syn_low32_elx()
{
    a_uart->write("syn_low32_elx\r\n");
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

extern "C" u8* __attribute__((weak)) _sbrk(int incr)
{
    return (u8*)0;
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
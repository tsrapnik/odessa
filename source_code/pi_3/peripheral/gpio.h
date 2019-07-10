#pragma once

#include "type_definitions.h"

class gpio
{
    public:
    //todo: move the enum to another file and maybe lose the singleton to get a more reusable design.
    //todo: maybe make a gpio interface that the real devices inherit, that way you can make some virtual gpio's just for testing stuff etc,
    //or to make sure code does not break when porting to another platform (raspberry pi 4?).
    //todo: do the same for all other devices.
    //represent all available gpio's.
    enum class device : u32
    {
        gpio_14 = 14,
        gpio_15 = 15,
        gpio_32 = 32,
        gpio_33 = 33
    };

    enum class pull_up_down_state : u32
    {
        disable_pull_up_or_down = 0,
        enable_pull_down = 1,
        enable_pull_up = 2
    };

    enum class function : u32
    {
        input = 0,
        output = 1,
        alternate_function_0 = 4,
        alternate_function_1 = 5,
        alternate_function_2 = 6,
        alternate_function_3 = 7,
        alternate_function_4 = 3,
        alternate_function_5 = 2,
    };

    private:
    struct registers
    {
        function gpfsel[6]; //0x00-0x14
        u32 reserved_0x18; //0x18
        u32 gpset0; //0x1c
        u32 reserved_0x20_0x24[2]; //0x20-0x24
        u32 gpclr0; //0x28
        u32 reserved_0x2c_0x30[2]; //0x2c-0x30
        u32 gplev0; //0x34
        u32 reserved_0x38_0x3c[2]; //0x38-0x3c
        u32 gpeds0; //0x40
        u32 reserved_0x44_0x48[2]; //0x44-0x48
        u32 gpren0; //0x4c
        u32 reserved_0x50_0x54[2]; //0x50-0x54
        u32 gpfen0; //0x58
        u32 reserved_0x5c_0x60[2]; //0x5c-0x60
        u32 gphen0; //0x64
        u32 reserved_0x68_0x6c[2]; //0x68-0x6c
        u32 gplen0; //0x70
        u32 reserved_0x74_0x78[2]; //0x74-0x78
        u32 gparen0; //0x7c
        u32 reserved_0x80_0x84[2]; //0x80-0x84
        u32 gpafend0; //0x88
        u32 reserved_0x8c_0x90[2]; //0x8c-0x90
        pull_up_down_state gppud;//0x94
        enum class gppudclk_options : u32
        {
            clear = 0,
            assert_clock_on_pin_x = 1
        }gppudclk[2]; //0x98-0x9c
    } __attribute__((packed));

    //keeps track of which devices are already used, so only
    //one instance of each can be created.
    static bool device_used[1];

    //base addresses of the different gpio device registers.
    // static constexpr registers* registers_base_address = {reinterpret_cast<registers*>(0x3F200000)};

    private:
    //remembers which gpio device this is.
    device device_id;

    pull_up_down_state the_pull_up_down_state;
    function the_function;

    //pointer to the actual registers of this device.
    volatile registers* the_registers = {reinterpret_cast<registers*>(0x3F200000)};

    //constructor is private, all objects should be created with
    //the create function, to avoid making multiple instances of
    //the same gpio device.
    gpio(device device_id, pull_up_down_state the_pull_up_down_state, function the_function);

    static void delay();
    void set_pull_up_down();
    void set_function();

    public:
    //destructor.
    ~gpio();

    //returns a pointer to a new device and initialises it. the
    //user should specify which device it should be, by passing
    //the correct device enum class. if the device is already in use
    //a nullptr will be returned.
    static gpio* create(device device_id, pull_up_down_state the_pull_up_down_state, function the_function);
};
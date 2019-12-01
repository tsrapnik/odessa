#pragma once

#include "type_definitions.h"

class gpio
{
    private:
    static constexpr usize device_count = 33;
    public:
    //todo: move the enum to another file and maybe lose the singleton to get a more reusable design.
    //todo: maybe make a gpio interface that the real devices inherit, that way you can make some virtual gpio's just for testing stuff etc,
    //or to make sure code does not break when porting to another platform (raspberry pi 4?).
    //todo: do the same for all other devices.
    //represent all available gpio's.
    enum class device : u32
    {
        gpio_5 = 5,
        gpio_6 = 6,
        gpio_7 = 7,
        gpio_8 = 8,
        gpio_10 = 10,
        gpio_11 = 11,
        gpio_14 = 14,
        gpio_15 = 15,
        gpio_18 = 18,
        gpio_19 = 19,
        gpio_20 = 20,
        gpio_21 = 21,
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
        w32 reserved_0x18; //0x18
        u32 gpset[2]; //0x1c-0x20
        w32 reserved_0x24; //0x24
        u32 gpclr[2]; //0x28-0x2c
        w32 reserved_0x30; //0x30
        u32 gplev[2]; //0x34-0x38
        w32 reserved_0x3c; //0x3c
        u32 gpeds[2]; //0x40-0x44
        w32 reserved_0x48; //0x48
        u32 gpren[2]; //0x4c-0x50
        w32 reserved_0x54; //0x54
        u32 gpfen[2]; //0x58-0x5c
        w32 reserved_0x60; //0x60
        u32 gphen[2]; //0x64-0x68
        w32 reserved_0x6c; //0x6c
        u32 gplen[2]; //0x70-0x74
        w32 reserved_0x78; //0x78
        u32 gparen[2]; //0x7c-0x80
        w32 reserved_0x84; //0x84
        u32 gpafen[2]; //0x88-0x8c
        w32 reserved_0x90; //0x90
        pull_up_down_state gppud;//0x94
        enum class gppudclk_options : u32
        {
            clear = 0,
            assert_clock_on_pin_x = 1
        }gppudclk[2]; //0x98-0x9c
    }  __attribute__((packed, aligned(4)));

    //keeps track of which devices are already used, so only
    //one instance of each can be created.
    static bool device_used[device_count];

    private:
    //remembers which gpio device this is.
    device device_id;

    //pointer to the actual registers of this device.
    volatile registers* the_registers = {reinterpret_cast<registers*>(0x3F200000)};

    //constructor is private, all objects should be created with
    //the create function, to avoid making multiple instances of
    //the same gpio device.
    gpio(device device_id, pull_up_down_state the_pull_up_down_state, function the_function);

    static void delay();
    void set_pull_up_down(pull_up_down_state the_pull_up_down_state);
    void set_function(function the_function);

    public:
    //destructor.
    ~gpio();

    //returns a pointer to a new device and initialises it. the
    //user should specify which device it should be, by passing
    //the correct device enum class. if the device is already in use
    //a nullptr will be returned.
    static gpio* create(device device_id, pull_up_down_state the_pull_up_down_state, function the_function);

    void set_output(bool set_high);
};
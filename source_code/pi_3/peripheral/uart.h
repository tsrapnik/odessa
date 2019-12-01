#pragma once

#include "gpio.h"
#include "type_definitions.h"
//todo: use reference manual register names everywhere.
class uart
{
    private:
    static constexpr u32 device_count = 1;

    public:
    //represent all available uart devices.
    enum class device
    {
        uart_pl011 = 0
    };

    private:
    struct registers
    {
        //todo: registers are not complete.
        u32 dr; //0x00
        w32 reserved_0[5]; //0x04-0x14
        enum class fr : u32 //0x18
        {
            rxfe = 1 << 4,
            txff = 1 << 5
        } fr;
        w32 reserved_1[2]; //0x1c-0x20
        u32 ibrd; //0x24
        u32 fbrd; //0x28
        enum class lcrh_options : u32 //0x2c
        {
            wlen8 = 3 << 5
        } lcrh;
        enum class cr_options : u32 //0x30
        {
            uart_enable = 1 << 0,
            txe = 1 << 8,
            rxe = 1 << 9
        } cr;
        u32 ifls; //0x34
        u32 imsc; //0x38
        u32 ris; //0x3c
        u32 mis; //0x40
        u32 icr; //0x44
    }  __attribute__((packed, aligned(4))); //todo: explain packed and aligned are necessary. especially aligned, because otherwise byte access in stead of word access.

    //keeps track of which devices are already used, so only
    //one instance of each can be created.
    static bool device_used[device_count];

    //base addresses of the different uart device registers.
    static constexpr registers* registers_base_address[device_count] = {reinterpret_cast<registers*>(0x3f201000)};

    private:
    //remembers which uart device this is.
    device device_id;

    //pointer to the actual registers of this device.
    volatile registers* the_registers;

    gpio* gpio_32;
    gpio* gpio_33;
    gpio* tx_pin;
    gpio* rx_pin;

    //constructor is private, all objects should be created with
    //the create function, to avoid making multiple instances of
    //the same uart device.
    uart(device device_id, gpio* gpio_32, gpio* gpio_33, gpio* tx_pin, gpio* rx_pin);

    public:
    //destructor.
    ~uart();

    //returns a pointer to a new device and initialises it. the
    //user should specify which device it should be, by passing
    //the correct device enum class. if the device is already in use
    //a nullptr will be returned.
    static uart* create(device device_id);

    void write(const char* string);

    //todo: use string class. provide byte array alternative also.
    void write(char* string, u32 size);

    //todo: set size automatically.
    char* read(u32& size);
};
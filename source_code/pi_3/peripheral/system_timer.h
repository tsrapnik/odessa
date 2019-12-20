#pragma once
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wbitfield-enum-conversion"

#include "device.h"
#include "type_definitions.h"
#include "volatile_operators.h"

//todo: use reference manual register names everywhere.
class system_timer
{
    private:
    static constexpr u32 timer_count = 4;

    struct registers
    {
        struct cs_struct
        {
            bool32 m0 : 1;
            bool32 m1 : 1;
            bool32 m2 : 1;
            bool32 m3 : 1;
            w32 reserved_4_31 : 28;
            volatile_assignment_operators(cs_struct, u32);
        } __attribute__((packed, aligned(4)))
        cs; //0x00

        u32 clo; //0x04

        u32 chi; //0x08

        u32 c[timer_count]; //0x0c-0x18
    } __attribute__((packed, aligned(4))); //todo: explain packed and aligned are necessary. especially aligned, because otherwise byte access in stead of word access.
    static_assert(sizeof(registers) == 0x1c, "system timer register map size does not match datasheet.");

    //keeps track if the device is already used.
    static bool device_used;

    private:
    //pointer to the actual registers of this device.
    volatile registers* the_registers = {reinterpret_cast<registers*>(0x3f003000)};

    //constructor is private, all objects should be created with
    //the create function, to avoid making multiple instances of
    //the same device.
    system_timer();

    public:
    //destructor.
    ~system_timer();

    //returns a pointer to a new device and initialises it. the
    //user should specify which device it should be, by passing
    //the correct device enum class. if the device is already in use
    //a nullptr will be returned.
    static system_timer* create();

    u64 get_system_time();
};

#pragma GCC diagnostic pop
#pragma once
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wbitfield-enum-conversion"

#include "type_definitions.h"
#include "volatile_operators.h"

//interface all interrupt sources should implement.
class interruptable
{
    public:
    virtual bool interrupt_occured() = 0;
    virtual void handle_interrupt() = 0;
};

class interrupt
{
    private:
    static constexpr usize device_count = 1;

    public:
    ///represent all available interrupt devices.
    enum class device
    {
        i2s_interrupt = 0,
    };

    private:
    ///available registers for each interrupt device.
    struct registers
    {
        //todo: provide a not implemented, not modifiable u32 for reserved and not yet implemented registers?
        u32 irq_basic_pending; //0x00
        u32 irq_pending[2]; //0x04-0x08
        u32 fiq_control; //0x0c
        u32 enable_irqs[2]; //0x10-0x14
        u32 enable_basic_irqs; //0x18
        u32 disable_irqs[2]; //0x1c-0x20
        u32 disable_basic_irqs; //0x24

    };
    static_assert(sizeof(registers) == 0x28, "interrupt register map size does not match datasheet.");

    ///
    ///keeps track of which devices are already used, so only
    ///one instance of each can be created.
    ///
    static bool device_used[device_count];

    ///
    ///base addresses of the different interrupt device registers.
    ///
    static constexpr registers* registers_base_address[device_count] = {reinterpret_cast<registers*>(0x3f00b200)};

    private:
    //remembers which interrupt device this is.
    device device_id;

    //pointer to the actual registers of this device.
    volatile registers* this_registers;

    //constructor is private, all objects should be created with
    //the create function, to avoid making multiple instances of
    //the same interrupt device.
    interrupt(device device_id, interruptable* source);

    public:
    ~interrupt();

    //returns a pointer to a new device and initialises it. the
    //user should specify which device it should be, by passing
    //the correct device enum class. if the device is already in use
    //a nullptr will be returned.
    static interrupt* create(device device_id, interruptable* source);
};

#pragma GCC diagnostic pop
#pragma once
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wbitfield-enum-conversion"

#include "list.h"
#include "type_definitions.h"
#include "volatile_operators.h"

class interrupt
{
    public:
    //the creation of an instance will disable interrupts. at the end of the scope in which the instance was
    //created the interrupt flags will be set back to their state before creation of the instance.
    //use as in example below.
    // {
    //     interrupt::disabler a_disabler;
    //     //from here till end of scope there are no interrupts.
    // }
    //do not use as below.
    // {
    //     interrupt::disabler a_disabler();
    //     //no constructor or destructor seem to get called.
    // }
    // {
    //     interrupt::disabler();
    //     //destructor gets called immediately after constructor and not at the end of the scope.
    // }
    class disabler
    {
        private:
        w32 flags;

        private:
        //private new operators prevent dynamic allocation, so only auto allocation is used. this guarantees the
        //destructor is called at the end of the scope.
        static void* operator new(usize);
        static void* operator new[](usize);

        public:
        disabler();
        ~disabler();
    };

    public:
    //interface all interrupt sources should implement.
    class interruptable
    {
        friend interrupt;
        private:
        virtual bool interrupt_occured() = 0;
        virtual void handle_interrupt() = 0;
    };

    private:
    static constexpr usize device_count = 2;

    public:
    ///represent all available interrupt devices.
    enum class device
    {
        i2s_interrupt = 0,
        uart_interrupt = 1,
    };

    private:
    ///available registers for each interrupt device.
    struct registers
    {
        u32 irq_basic_pending; //0x00
        u32 irq_pending[2]; //0x04-0x08
        u32 fiq_control; //0x0c
        u32 enable_irqs[2]; //0x10-0x14
        u32 enable_basic_irqs; //0x18
        u32 disable_irqs[2]; //0x1c-0x20
        u32 disable_basic_irqs; //0x24

    } __attribute__((packed, aligned(4)));
    static_assert(sizeof(registers) == 0x28, "interrupt register map size does not match datasheet.");

    ///
    ///keeps track of which devices are already used, so only
    ///one instance of each can be created.
    ///
    static bool device_used[device_count];

    ///
    ///base addresses of the different interrupt device registers.
    ///
    static volatile registers* const the_registers;

    private:
    //remembers which interrupt device this is.
    device device_id;

    //source of the interrupt, which is used to handle the interrupt.
    interruptable* source;

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

    //handles all interrupts that where created.
    static void handle();

    private:
    static void enable_interrupts();
    static void disable_interrupts();
    static void push_interrupt_state();
    static void pop_interrupt_state();
};

#pragma GCC diagnostic pop
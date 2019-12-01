#include "interrupt.h"
#include "uart.h"

bool interrupt::device_used[device_count] = {false};
constexpr interrupt::registers* interrupt::registers_base_address[device_count];

interrupt::interrupt(device device_id, interruptable* source)
{
    //mark the device as used to avoid another instance of this device can be made.
    device_used[static_cast<u32>(device_id)] = true;

    //set the device_id, so it can be checked at runtime which device this is.
    this->device_id = device_id;

    the_registers = registers_base_address[static_cast<u32>(device_id)];

    switch (device_id)
    {
    case device::i2s_interrupt:
        //todo: do for all interrupt.
        constexpr usize pcm_int = 55;
        the_registers->enable_irqs[pcm_int / 32u] = 1u << (pcm_int % 32u);
        break;
    }
}

interrupt::~interrupt()
{
    //unmark the device as used, so it can be created again when needed.
    device_used[static_cast<u32>(device_id)] = false;

    //todo: disable interrupt again.
}

interrupt* interrupt::create(device device_id, interruptable* source)
{
    //check if the device isn't already used, if so return a nullptr.
    //else create the device and return its pointer.
    if(device_used[static_cast<u32>(device_id)])
        return nullptr;
    else
    {
        //todo: handle nullptr return of new at all uses of new. maybe implement variant return type?
        interrupt* new_device = new interrupt(device_id, source);
        return new_device;
    }
}

//interrupt handlers.
//todo: replace with global logger class.
extern uart* a_uart;

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
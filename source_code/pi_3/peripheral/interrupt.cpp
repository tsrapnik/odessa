#include "interrupt.h"
#include "list_iterator.h"
#include "memory.h"
#include "debugger.h"
#include "assert.h"

interrupt::disabler::disabler()
{
    //save original state.
    asm volatile("mrs %0, daif"
                 : "=r"(flags));
    //disable interrupts only irq (fiq are not used).
    asm volatile("msr DAIFSet, #2");

    memory::data_memory_barrier();
}

interrupt::disabler::~disabler()
{
    memory::data_memory_barrier();

    //set back to previous state.
    asm volatile("msr daif, %0" ::"r"(flags));
}

//list of all interrupt sources that were created and can trigger an interrupt.
static list<interrupt::interruptable*> sources;

bool interrupt::device_used[device_count] = {false};

volatile interrupt::registers* const interrupt::the_registers = reinterpret_cast<registers*>(0x3f00b200);

interrupt::interrupt(device device_id, interruptable* source) :
    source(source)
{
    //mark the device as used to avoid another instance of this device can be made.
    device_used[static_cast<u32>(device_id)] = true;

    //set the device_id, so it can be checked at runtime which device this is.
    this->device_id = device_id;

    sources.append_copy(source);

    u32 interrupt_index;
    switch(device_id)
    {
        case device::i2s_interrupt:
            //todo: do for all interrupt.
            interrupt_index = 55;
            break;
        case device::uart_interrupt:
            interrupt_index = 57;
            break;
        default:
            interrupt_index = 0;
            assert(false);
            break;
    }
    the_registers->enable_irqs[interrupt_index / 32u] = 1u << (interrupt_index % 32u);
}

interrupt::~interrupt()
{
    //remove the source from the sources list.
    list_iterator<interruptable*> sources_iterator(sources);
    for(sources_iterator.to_first(); !sources_iterator.at_end(); sources_iterator++)
    {
        if(sources_iterator.get_data_copy() == source)
        {
            sources_iterator.delete_current();
            break;
        }
    }

    //unmark the device as used, so it can be created again when needed.
    device_used[static_cast<u32>(device_id)] = false;

    //todo: deinit interrupt.
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

void interrupt::handle()
{
    //we just read the pending registers, so they are acknowledged.
    the_registers->irq_basic_pending = the_registers->irq_basic_pending;
    the_registers->irq_pending[0] = the_registers->irq_pending[0];
    the_registers->irq_pending[1] = the_registers->irq_pending[1];

    //check for each created interrupt source if an interrupt occured, if so handle it.
    list_iterator<interruptable*> sources_iterator(sources);
    for(sources_iterator.to_first(); !sources_iterator.at_end(); sources_iterator++)
    {
        interruptable* source = sources_iterator.get_data_copy();
        if(source->interrupt_occured())
        {
            source->handle_interrupt();
        }
    }
}

//interrupt handlers.

extern "C" void irq_cur_el0()
{
    //not used for the moment, so we write a log and just hang for debugging purposes.
    debugger::print("irq_cur_el0\r\n");
    while(true)
        ;
}
extern "C" void fiq_cur_el0()
{
    //not used for the moment, so we write a log and just hang for debugging purposes.
    debugger::print("fiq_cur_el0\r\n");
    while(true)
        ;
}
extern "C" void irq_cur_elx(void)
{
    //in aarch64 by default interrupts are disabled on entering an interrupt. this means
    //you can only have nested interrupts if you explicitely reenable interrupts in the
    //interrupt routine. nested interrupts complicate things a lot and we don't need them
    //so we leave it as it is.
    interrupt::handle();
}
extern "C" void fiq_cur_elx()
{
    //not used for the moment, so we write a log and just hang for debugging purposes.
    debugger::print("fiq_cur_elx\r\n");
    while(true)
        ;
}
extern "C" void irq_low64_elx()
{
    //not used for the moment, so we write a log and just hang for debugging purposes.
    debugger::print("irq_low64_elx\r\n");
    while(true)
        ;
}
extern "C" void fiq_low64_elx()
{
    //not used for the moment, so we write a log and just hang for debugging purposes.
    debugger::print("fiq_low64_elx\r\n");
    while(true)
        ;
}
extern "C" void irq_low32_elx()
{
    //not used for the moment, so we write a log and just hang for debugging purposes.
    debugger::print("irq_low32_elx\r\n");
    while(true)
        ;
}
extern "C" void fiq_low32_elx()
{
    //not used for the moment, so we write a log and just hang for debugging purposes.
    debugger::print("fiq_low32_elx\r\n");
    while(true)
        ;
}
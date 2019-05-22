#include "gpio.h"
#include "enum_flags.h"

//todo: remove.
#include "uart.h"
#include "string.h"
extern uart* a_uart;

bool gpio::device_used[1] = {false};
// constexpr gpio::registers* gpio::registers_base_address[1];

gpio::gpio(device device_id, pull_up_down_state the_pull_up_down_state, function the_function):
    the_pull_up_down_state(the_pull_up_down_state),
    the_function(the_function)
{
    //mark the device as used to avoid another instance of this device can be made.
    device_used[static_cast<u32>(device_id)] = true;

    //set the device_id, so it can be checked at runtime which device this is.
    this->device_id = device_id;

    set_pull_up_down();
    set_function();
}

//just makes sure cpu hangs for at least 150 cycles.
void gpio::delay()
{
    for(volatile u32 count = 0; count < 150; count++)
    {
    }
}

void gpio::set_pull_up_down()
{
    volatile registers::gppudclk_options* a_gppudclk_pointer = &the_registers->gppudclk[static_cast<u32>(device_id) / 32];

    the_registers->gppud = the_pull_up_down_state;
    // // a_uart->write("set pud.\r\n", 10);
    // // a_uart->write(string::to_string(reinterpret_cast<usize>(&the_registers->gppud)), 19);
    // // a_uart->write("\r\n", 2);
    // // a_uart->write(string::to_string(static_cast<usize>(the_pull_up_down_state)), 19);
    // // a_uart->write("\r\n", 2);
    delay();
    *a_gppudclk_pointer = registers::gppudclk_options::assert_clock_on_pin_x << (static_cast<u32>(device_id) % 32);
    // // a_uart->write("set gpud.\r\n", 11);
    // // a_uart->write(string::to_string(reinterpret_cast<usize>(a_gppudclk_pointer)), 19);
    // // a_uart->write("\r\n", 2);
    // // a_uart->write(string::to_string(static_cast<usize>(registers::gppudclk_options::assert_clock_on_pin_x << (static_cast<u32>(device_id) % 32))), 19);
    // // a_uart->write("\r\n", 2);
    delay();
    the_registers->gppud = pull_up_down_state::disable_pull_up_or_down;
    *a_gppudclk_pointer = registers::gppudclk_options::clear;
}

void gpio::set_function()
{
    // nSelReg = ARM_GPIO_GPFSEL0 + (15 / 10) * 4;
    // nShift = (15 % 10) * 3;

    // nValue = *(u32 volatile*)nSelReg;
    // nValue &= ~(7 << nShift);
    // nValue |= 4 << nShift;
    // *(u32 volatile*)nSelReg = nValue;
    u32 shift = (static_cast<u32>(device_id) % 10) * 3;
    volatile function* a_function_pointer = &(the_registers->gpfsel[static_cast<u32>(device_id) / 10]);

    // a_uart->write("red fun.\r\n", 10);
    // a_uart->write("set fun.\r\n", 10);
    a_uart->write(string::to_string(reinterpret_cast<usize>(a_function_pointer)), 19);
    a_uart->write(string::to_string(static_cast<usize>(*a_function_pointer)), 19);
    // a_uart->write("\r\n", 2);

    // a_function_pointer = (volatile function*)0x3f200004;
    // a_uart->write(string::to_string(reinterpret_cast<usize>(a_function_pointer)), 19);
    // a_uart->write("\r\n", 2);

    // a_uart->write("\r\n", 2);
    // a_uart->write("\r\n", 2);
    function a_function = *a_function_pointer;
    // a_uart->write(string::to_string(static_cast<usize>(*a_function_pointer)), 19);
    // a_uart->write("\r\n", 2);
    a_function &= ~(function::alternate_function_3 << shift);
    // // a_uart->write(string::to_string(static_cast<usize>(a_function)), 19);
    // // a_uart->write("\r\n", 2);
    a_function |= the_function << shift;
    // // a_uart->write(string::to_string(static_cast<usize>(a_function)), 19);
    // // a_uart->write("\r\n", 2);
    // // a_uart->write("\r\n", 2);

    *a_function_pointer = a_function;
    // a_uart->write("set fun.\r\n", 10);
    // a_uart->write(string::to_string(reinterpret_cast<usize>(a_function_pointer)), 19);
    // a_uart->write("\r\n", 2);
    // a_uart->write(string::to_string(static_cast<usize>(a_function)), 19);
    // a_uart->write("\r\n", 2);
    // a_uart->write("\r\n", 2);
}

gpio::~gpio()
{
    //unmark the device as used, so it can be created again when needed.
    device_used[static_cast<u32>(device_id)] = false;

    //todo: make sure device gets destroyed.
}

gpio* gpio::create(device device_id, pull_up_down_state the_pull_up_down_state, function the_function)
{
    //check if the device isn't already used, if so return a nullptr.
    //else create the device and return its pointer.
    if(device_used[static_cast<u32>(device_id)])
        return nullptr;
    else
    {
        gpio* new_device = new gpio(device_id, the_pull_up_down_state, the_function);
        return new_device;
    }
}

void gpio::write(char* string, u32 size)
{
    for(u32 index = 0; index < 8; index++)
    {
        // while(static_cast<bool>(the_registers->fr & registers::fr::txff))
        // {
        // }
        // the_registers->dr = string[index];
    }
}
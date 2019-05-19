#include "uart.h"
#include "enum_flags.h"

bool uart::device_used[1] = {false};
constexpr uart::registers* uart::registers_base_address[1];

uart::uart(device device_id)
{
    //mark the device as used to avoid another instance of this device can be made.
    device_used[static_cast<u32>(device_id)] = true;

    //set the device_id, so it can be checked at runtime which device this is.
    this->device_id = device_id;

    this_registers = registers_base_address[static_cast<u32>(device_id)];

    u32 clock_rate = 48000000;
    u32 baud_rate_times_16 = 115200 * 16;
    u32 fraction = (clock_rate % baud_rate_times_16) * 8 / 115200;

    this_registers->imsc = 0;
    this_registers->icr = 0x7ff;
    this_registers->ibrd = clock_rate / baud_rate_times_16;
    this_registers->fbrd = fraction / 2 + fraction % 2;
    this_registers->lcrh = registers::lcrh_options::wlen8;
    this_registers->ifls = 0;
    this_registers->cr = registers::cr_options::uart_enable |
                         registers::cr_options::txe |
                         registers::cr_options::rxe;
}

uart::~uart()
{
    //unmark the device as used, so it can be created again when needed.
    device_used[(u32)device_id] = false;

    //todo: make sure device gets destroyed.
}

uart* uart::create(device device_id)
{
    //check if the device isn't already used, if so return a nullptr.
    //else create the device and return its pointer.
    if(device_used[(u32)device_id])
        return nullptr;
    else
    {
        uart* new_device = new uart(device_id);
        return new_device;
    }
}

void uart::write(char* string, u32 size)
{
    for(u32 index = 0; index < 8; index++)
    {
        while(static_cast<bool>(this_registers->fr & registers::fr::txff))
        {
        }
        this_registers->dr = string[index];
    }
}
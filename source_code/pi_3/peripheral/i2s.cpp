#include "i2s.h"
#include "enum_flags.h"

bool i2s::device_used[device_count] = {false};
constexpr i2s::registers* i2s::registers_base_address[device_count];

i2s::i2s(device device_id)
{
    //mark the device as used to avoid another instance of this device can be made.
    device_used[static_cast<u32>(device_id)] = true;

    //set the device_id, so it can be checked at runtime which device this is.
    this->device_id = device_id;

    this_registers = registers_base_address[static_cast<u32>(device_id)];

    this_registers->data_fifo = this_registers->data_fifo ^ registers::data_fifo_options::data_mask;
    this_registers->data_fifo ^= registers::data_fifo_options::data_mask;
}

i2s::~i2s()
{
    //unmark the device as used, so it can be created again when needed.
    device_used[static_cast<u32>(device_id)] = false;
}

i2s* i2s::create(device device_id)
{
    //check if the device isn't already used, if so return a nullptr.
    //else create the device and return its pointer.
    if(device_used[static_cast<u32>(device_id)])
        return nullptr;
    else
    {
        i2s* new_device = new i2s(device_id);
        return new_device;
    }
    this_registers->control = registers::control_options::i2s_enable;
}

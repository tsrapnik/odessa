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

    this_registers->control_and_status.enable_pcm = true;

    //todo: set all options.

    registers::control_and_status_struct temp_control_and_status;

    temp_control_and_status = this_registers->control_and_status;
    temp_control_and_status.clear_rx_fifo = true;
    temp_control_and_status.clear_tx_fifo = true;
    temp_control_and_status.pcm_clock_sync_helper = true;

    this_registers->control_and_status = temp_control_and_status;

    while(!this_registers->control_and_status.pcm_clock_sync_helper)
    {
    }

    temp_control_and_status = this_registers->control_and_status;
    temp_control_and_status.rx_threshold = registers::control_and_status_struct::rx_threshold_enum::fifo_single_sample;
    temp_control_and_status.tx_threshold = registers::control_and_status_struct::tx_threshold_enum::fifo_full_but_one;

    this_registers->control_and_status = temp_control_and_status;
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
}

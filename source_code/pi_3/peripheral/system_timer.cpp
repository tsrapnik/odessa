#include "system_timer.h"
#include "assert.h"

bool system_timer::device_used = false;

system_timer::system_timer()
{
    //mark the device as used to avoid another instance of this device can be made.
    device_used = true;
}

system_timer::~system_timer()
{
    //todo: make sure device gets destroyed and deinitialized.
    device_used = false;
}

system_timer* system_timer::create()
{
    //check if the device isn't already used, if so return a nullptr.
    //else create the device and return its pointer.
    if(device_used)
        return nullptr;
    else
    {
        system_timer* new_device = new system_timer();
        return new_device;
    }
}

u64 system_timer::get_system_time()
{
    //todo: make sure the clo did not just change after reading the cli (happens around every hour).
    return static_cast<u64>(the_registers->clo) | (static_cast<u64>(the_registers->chi) << 32);
}
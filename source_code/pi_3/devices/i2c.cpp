#include "i2c.h"
#include "enum_flags.h"

bool i2c::device_used[ 3 ] = { false, false, false };
constexpr i2c::registers* i2c::registers_base_address[ 3 ];

i2c::i2c( device device_id )
{
    //mark the device as used to avoid another instance of this device can be made.
    device_used[ ( u32 )device_id ] = true;

    //set the device_id, so it can be checked at runtime which device this is.
    this->device_id = device_id;

    this_registers = registers_base_address[ ( u32 )device_id ];
    
    this_registers->data_fifo = this_registers->data_fifo ^ registers::data_fifo_options::data_mask;
    this_registers->data_fifo ^= registers::data_fifo_options::data_mask;
}

i2c::~i2c()
{
    //unmark the device as used, so it can be created again when needed.
    device_used[ ( u32 ) device_id ] = false;
}

i2c* i2c::create( device device_id )
{
    //check if the device isn't already used, if so return a nullptr.
    //else create the device and return its pointer.
    if( device_used[ ( u32 ) device_id ] )
        return nullptr;
    else
    {
        i2c* new_device = new i2c( device_id );
        return new_device;
    }
    this_registers->control = registers::control_options::i2c_enable;
}

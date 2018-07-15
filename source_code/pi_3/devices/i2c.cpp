#include "i2c.h"
#include "enum_flags.h"

bool i2c::deviceUsed[ 3 ] = { false, false, false };
constexpr i2c::registers* i2c::registersBaseAddress[ 3 ];

i2c::i2c( device deviceId )
{
    //Mark the device as used to avoid another instance of this device can be made.
    deviceUsed[ ( int )deviceId ] = true;

    //Set the deviceId, so it can be checked at runtime which device this is.
    this->deviceId = deviceId;

    this_registers = registersBaseAddress[ ( int )deviceId ];
    
    this_registers->dataFifo = this_registers->dataFifo ^ registers::dataFifoOptions::dataMask;
    this_registers->dataFifo ^= registers::dataFifoOptions::dataMask;
}

i2c::~i2c()
{
    //Unmark the device as used, so it can be created again when needed.
    deviceUsed[ ( int ) deviceId ] = false;
}

i2c* i2c::create( device deviceId )
{
    //Check if the device isn't already used, if so return a nullptr.
    //Else create the device and return its pointer.
    if( deviceUsed[ ( int ) deviceId ] )
        return nullptr;
    else
    {
        i2c* newDevice = new i2c( deviceId );
        return newDevice;
    }
    this_registers->control = registers::controlOptions::i2cEnable;
}

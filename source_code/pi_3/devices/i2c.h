#pragma once

#include "typeDefinitions.h"

class i2c
{
public:
    ///
    ///Represent all available i2c devices.
    ///
    enum class device
    {
        i2c0 = 0,
        i2c1 = 1,
        i2c2 = 2
    };

private:
    ///
    ///Available registers for each i2c device.
    ///
    struct registers
    {        
        ///
        ///Control register options.
        ///
        enum class controlOptions: u32
        {
            i2cEnable = 1 << 15,
            interruptOnRx = 1 << 10,
            interruptOnTx = 1 << 9,
            interruptOnDone = 1 << 8,
            startTransfer = 1 << 7,
            clearFifo = 1 << 4,
            readTransfer = 1 << 0
        };

        ///
        ///Status register options.
        ///
        enum class statusOptions: u32
        {
            clockStretchTimeout = 1 << 9,
            acknowledgeError = 1 << 8,
            rxFifoFull = 1 << 7,
            txFifoEmpty = 1 << 6,
            rxFifoContainsData = 1 << 5,
            txFifoCanAcceptData = 1 << 4,
            rxFifoNeedsReading = 1 << 3,
            txFifoNeedsWriting = 1 << 2,
            transferDone = 1 << 1,
            transferActive = 1 << 0
        };
        
        ///
        ///DataLength register options.
        ///
        enum class dataLengthOptions: u32
        {
            dataLengthMask = 0xffff,
            dataLengthOffset = 0
        };

        ///
        ///SlaveAddress register options.
        ///
        enum class slaveAddressOptions: u32
        {
            slaveAddressMask = 0x7f,
            slaveAddressOffset = 0
        };

        ///
        ///DataFifo register options.
        ///
        enum class dataFifoOptions: u32
        {
            dataMask = 0xff,
            dataOffset = 0
        };

        ///
        ///ClockDivider register options.
        ///
        enum class clockDividerOptions: u32
        {
            clockDividerMask = 0xffff,
            clockDividerOffset = 0
        };

        ///
        ///DataDelay register options.
        ///
        enum class dataDelayOptions: u32
        {
            fallingEdgeDelayMask = 0xffff0000,
            fallingEdgeDelayOffset = 16,
            risingEdgeDelayMask = 0x0000ffff,
            risingEdgeDelayOffset = 0
        };

        ///
        ///ClockStretchTimeout register options.
        ///
        enum class clockStretchTimeoutOptions: u32
        {
            clockStretchTimeoutValueMask = 0xffff,
            clockStretchTimeoutValueOffset = 0
        };
        volatile controlOptions control; //0x00
        volatile statusOptions status; //0x04
        volatile dataLengthOptions dataLength; //0x08
        volatile slaveAddressOptions slaveAddress; //0x0c
        volatile dataFifoOptions dataFifo; //0x10
        volatile clockDividerOptions clockDivider; //0x14
        volatile dataDelayOptions dataDelay; //0x18
        volatile clockStretchTimeoutOptions clockStretchTimeout; //0x1c
    };
    
    ///
    ///Keeps track of which devices are already used, so only
    ///one instance of each can be created.
    ///
    static bool deviceUsed[ 3 ];

    ///
    ///Base addresses of the different i2c device registers.
    ///
    static constexpr registers* registersBaseAddress[ 3 ] = {   reinterpret_cast< registers* >( 0x7e205000 ),
                                                                reinterpret_cast< registers* >( 0x7e804000 ),
                                                                reinterpret_cast< registers* >( 0x7e805000 ) };

private:
    ///
    ///Remembers which i2c device this is.
    ///
    device deviceId;

    ///
    ///Pointer to the actual registers of this device.
    ///
    registers* this_registers;

    ///
    ///Constructor is private, all objects should be created with
    ///the create function, to avoid making multiple instances of
    ///the same i2c device.
    ///
    i2c( device deviceId );

public:
    ///
    ///Destructor.
    ///
    ~i2c();

    ///
    ///Returns a pointer to a new device and initialises it. The
    ///user should specify which device it should be, by passing 
    ///the correct device enum class. If the device is already in use
    ///a nullptr will be returned.
    ///
    i2c* create( device deviceId );
};
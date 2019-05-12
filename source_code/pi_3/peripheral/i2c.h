#pragma once

#include "type_definitions.h"

class i2c
{
public:
    ///
    ///represent all available i2c devices.
    ///
    enum class device
    {
        i2c0 = 0,
        i2c1 = 1,
        i2c2 = 2
    };

private:
    ///
    ///available registers for each i2c device.
    ///
    struct registers
    {        
        ///
        ///control register options.
        ///
        enum class control_options: u32
        {
            i2c_enable = 1 << 15,
            interrupt_on_rx = 1 << 10,
            interrupt_on_tx = 1 << 9,
            interrupt_on_done = 1 << 8,
            start_transfer = 1 << 7,
            clear_fifo = 1 << 4,
            read_transfer = 1 << 0
        };

        ///
        ///status register options.
        ///
        enum class status_options: u32
        {
            clock_stretch_timeout = 1 << 9,
            acknowledge_error = 1 << 8,
            rx_fifo_full = 1 << 7,
            tx_fifo_empty = 1 << 6,
            rx_fifo_contains_data = 1 << 5,
            tx_fifo_can_accept_data = 1 << 4,
            rx_fifo_needs_reading = 1 << 3,
            tx_fifo_needs_writing = 1 << 2,
            transfer_done = 1 << 1,
            transfer_active = 1 << 0
        };
        
        ///
        ///data_length register options.
        ///
        enum class data_length_options: u32
        {
            data_length_mask = 0xffff,
            data_length_offset = 0
        };

        ///
        ///slave_address register options.
        ///
        enum class slave_address_options: u32
        {
            slave_address_mask = 0x7f,
            slave_address_offset = 0
        };

        ///
        ///data_fifo register options.
        ///
        enum class data_fifo_options: u32
        {
            data_mask = 0xff,
            data_offset = 0
        };

        ///
        ///clock_divider register options.
        ///
        enum class clock_divider_options: u32
        {
            clock_divider_mask = 0xffff,
            clock_divider_offset = 0
        };

        ///
        ///data_delay register options.
        ///
        enum class data_delay_options: u32
        {
            falling_edge_delay_mask = 0xffff0000,
            falling_edge_delay_offset = 16,
            rising_edge_delay_mask = 0x0000ffff,
            rising_edge_delay_offset = 0
        };

        ///
        ///clock_stretch_timeout register options.
        ///
        enum class clock_stretch_timeout_options: u32
        {
            clock_stretch_timeout_value_mask = 0xffff,
            clock_stretch_timeout_value_offset = 0
        };
        volatile control_options control; //0x00
        volatile status_options status; //0x04
        volatile data_length_options data_length; //0x08
        volatile slave_address_options slave_address; //0x0c
        volatile data_fifo_options data_fifo; //0x10
        volatile clock_divider_options clock_divider; //0x14
        volatile data_delay_options data_delay; //0x18
        volatile clock_stretch_timeout_options clock_stretch_timeout; //0x1c
    };
    
    ///
    ///keeps track of which devices are already used, so only
    ///one instance of each can be created.
    ///
    static bool device_used[ 3 ];

    ///
    ///base addresses of the different i2c device registers.
    ///
    static constexpr registers* registers_base_address[ 3 ] = {   reinterpret_cast< registers* >( 0x7e205000 ),
                                                                reinterpret_cast< registers* >( 0x7e804000 ),
                                                                reinterpret_cast< registers* >( 0x7e805000 ) };

private:
    ///
    ///remembers which i2c device this is.
    ///
    device device_id;

    ///
    ///pointer to the actual registers of this device.
    ///
    registers* this_registers;

    ///
    ///constructor is private, all objects should be created with
    ///the create function, to avoid making multiple instances of
    ///the same i2c device.
    ///
    i2c( device device_id );

public:
    ///
    ///destructor.
    ///
    ~i2c();

    ///
    ///returns a pointer to a new device and initialises it. the
    ///user should specify which device it should be, by passing 
    ///the correct device enum class. if the device is already in use
    ///a nullptr will be returned.
    ///
    i2c* create( device device_id );
};
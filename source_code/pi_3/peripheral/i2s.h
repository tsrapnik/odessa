#pragma once
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wbitfield-enum-conversion"

#include "type_definitions.h"
#include "volatile_operators.h"

class i2s
{
    private:
    static constexpr u32 device_count = 1;

    public:
    ///represent all available i2s devices.
    enum class device
    {
        i2s0 = 0,
    };

    private:
    ///available registers for each i2s device.
    struct registers
    {
        struct control_and_status_struct
        {
            bool enable_pcm : 1;
            bool enable_reception : 1;
            bool enable_transmission : 1;
            bool clear_tx_fifo : 1;
            bool clear_rx_fifo : 1;
            enum class tx_threshold_enum : u32
            {
                fifo_empty = 0,
                fifo_less_than_full_0 = 1,
                fifo_less_than_full_1 = 2,
                fifo_full_but_one = 3,
            } tx_threshold : 2;
            enum class rx_threshold_enum : u32
            {
                fifo_single_sample = 0,
                fifo_at_leas_full = 1,
                fifo_at_least = 2,
                fifo_full = 3,
            } rx_threshold : 2;
            bool enable_dma_request : 1;
            u32 reserved_10_12 : 3;
            bool tx_fifo_in_sync : 1;
            bool fx_fifo_in_sync : 1;
            bool tx_fifo_has_error : 1;
            bool rx_fifo_has_error : 1;
            bool tx_needs_writing : 1;
            bool rx_needs_reading : 1;
            bool tx_can_accept_data : 1;
            bool rx_contains_data : 1;
            bool tx_fifo_empty : 1;
            bool rx_fifo_full : 1;
            bool rx_sign_extend : 1;
            bool pcm_clock_sync_helper : 1;
            bool ram_not_in_standby : 1;
            u32 reserved_26_31 : 6;

            volatile_assignment_operators(control_and_status_struct, u32);
        } control_and_status; //0x00

        u32 fifo_data; //0x04

        struct mode_struct
        {
            u32 frame_sync_length : 10;
            u32 frame_length : 10;
            bool frame_sync_invert : 1;
            enum class frame_sync_mode_enum : u32
            {
                master = 0,
                slave = 1,
            } frame_sync_mode : 1;
            bool clock_invert : 1;
            enum class clock_mode_enum : u32
            {
                master = 0,
                slave = 1,
            } clock_mode : 1;
            enum class transmit_frame_packed_mode_enum : u32
            {
                single_channel = 0,
                split = 1,
            } transmit_frame_packed_mode : 1;
            enum class receive_frame_packed_mode_enum : u32
            {
                single_channel = 0,
                merged = 1,
            } receive_frame_packed_mode : 1;
            bool pdm_input_mode_enable : 1;
            enum class pdm_decimation_factor_enum : u32
            {
                factor_16 = 0,
                factor_32 = 1,
            } pdm_decimation_factor : 1;
            bool pcm_clock_disable : 1;
            u32 reserved_29_31 : 3;

            volatile_assignment_operators(mode_struct, u32);
        } mode; //0x08

        struct configuration_struct
        {
            u32 channel_2_width : 4;
            u32 channel_2_position : 10;
            bool channel_2_enable : 1;
            bool channel_2_width_extension : 1;
            u32 channel_1_width : 4;
            u32 channel_1_position : 10;
            bool channel_1_enable : 1;
            bool channel_1_width_extension : 1;

            volatile_assignment_operators(configuration_struct, u32);
        };

        configuration_struct receive_configuration; //0x0c

        configuration_struct transmit_configuration; //0x10

        struct dma_request_level_struct
        {
            u32 rx_request_level : 7;
            u32 reserved_7 : 1;
            u32 tx_request_level : 7;
            u32 reserved_15 : 1;
            u32 rx_panic_level : 7;
            u32 reserved_23 : 1;
            u32 tx_panic_level : 7;
            u32 reserved_31 : 1;

            volatile_assignment_operators(dma_request_level_struct, u32);
        } dma_request_level; //0x14

        struct interrupt_enables_struct
        {
            bool tx_write_interrupt_enable : 1;
            bool rx_read_interrupt_enable : 1;
            bool tx_error_interrupt_enable : 1;
            bool rx_error_interrupt_enable : 1;
            u32 reserved_4_31 : 28;

            volatile_assignment_operators(interrupt_enables_struct, u32);
        } interrupt_enables; //0x18

        struct interrupt_status_and_clear_struct
        {
            enum class status_and_clear
            {
                no_interrupt_occured = 0,
                interrupt_occured = 1,
                clear_flag = 1,
            };

            status_and_clear tx_write_interrupt : 1;
            status_and_clear rx_read_interrupt : 1;
            status_and_clear tx_error_interrupt : 1;
            status_and_clear rx_error_interrupt : 1;
            u32 reserved_4_31 : 28;

            volatile_assignment_operators(interrupt_status_and_clear_struct, u32);
        } interrupt_status_and_clear; //0x1c

        struct gray_mode_control_struct
        {
            bool enable_gray_mode : 1;
            bool clear_gray_mode_logic : 1;
            bool flush_rx_buffer : 1;
            bool reserved_3 : 1;
            u32 current_fill_level : 6;
            u32 bits_flushed : 6;
            u32 current_level : 6;
            u32 reserved_22_31 : 10;

            volatile_assignment_operators(gray_mode_control_struct, u32);
        } gray_mode_control; //0x20
    };

    ///
    ///keeps track of which devices are already used, so only
    ///one instance of each can be created.
    ///
    static bool device_used[device_count];

    ///
    ///base addresses of the different i2s device registers.
    ///
    static constexpr registers* registers_base_address[device_count] = {reinterpret_cast<registers*>(0x00000000)};

    private:
    ///
    ///remembers which i2s device this is.
    ///
    device device_id;

    ///
    ///pointer to the actual registers of this device.
    ///
    volatile registers* this_registers;

    ///
    ///constructor is private, all objects should be created with
    ///the create function, to avoid making multiple instances of
    ///the same i2s device.
    ///
    i2s(device device_id);

    public:
    ///
    ///destructor.
    ///
    ~i2s();

    ///
    ///returns a pointer to a new device and initialises it. the
    ///user should specify which device it should be, by passing
    ///the correct device enum class. if the device is already in use
    ///a nullptr will be returned.
    ///
    i2s* create(device device_id);
};

#pragma GCC diagnostic pop
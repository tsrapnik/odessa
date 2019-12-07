#pragma once
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wbitfield-enum-conversion"

#include "gpio.h"
#include "type_definitions.h"
#include "volatile_operators.h"

class i2s
{
    private:
    static constexpr usize device_count = 1;

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
        struct cs_a_struct
        {
            bool en : 1;
            bool rxon : 1;
            bool txon : 1;
            bool txclr : 1;
            bool rxclr : 1;
            enum class txthr_enum : u32
            {
                fifo_empty = 0,
                fifo_less_than_full_0 = 1,
                fifo_less_than_full_1 = 2,
                fifo_full_but_one = 3,
            } txthr : 2;
            enum class rxthr_enum : u32
            {
                fifo_single_sample = 0,
                fifo_at_leas_full = 1,
                fifo_at_least = 2,
                fifo_full = 3,
            } rxthr : 2;
            bool dmaen : 1;
            w32 reserved_10_12 : 3;
            bool txsync : 1;
            bool rxsync : 1;
            bool txerr : 1;
            bool rxerr : 1;
            bool txh : 1;
            bool rxr : 1;
            bool txd : 1;
            bool rxd : 1;
            bool txe : 1;
            bool rxf : 1;
            bool rxsex : 1;
            bool sync : 1;
            enum class stdby_enum : u32
            {
                enable = 0,
                disable = 1,
            } stby : 1;
            w32 reserved_26_31 : 6;

            volatile_assignment_operators(cs_a_struct, u32);
        } __attribute__((packed, aligned(4)))
        cs_a; //0x00

        i32 fifo_a; //0x04

        struct mode_a_struct
        {
            u32 fslen : 10;
            u32 flen : 10;
            bool fsi : 1;
            enum class fsm_enum : u32
            {
                master = 0,
                slave = 1,
            } fsm : 1;
            bool clki : 1;
            enum class clkm_enum : u32
            {
                master = 0,
                slave = 1,
            } clkm : 1;
            enum class ftxp_enum : u32
            {
                single_channel = 0,
                split = 1,
            } ftxp : 1;
            enum class frxp_enum : u32
            {
                single_channel = 0,
                merged = 1,
            } frxp : 1;
            bool pdme : 1;
            enum class pdmn_enum : u32
            {
                factor_16 = 0,
                factor_32 = 1,
            } pdmn : 1;
            bool clk_dis : 1;
            w32 reserved_29_31 : 3;

            volatile_assignment_operators(mode_a_struct, u32);
        } __attribute__((packed, aligned(4)))
        mode_a; //0x08

        struct xc_a_struct
        {
            u32 ch2wid : 4;
            u32 ch2pos : 10;
            bool ch2en : 1;
            bool ch2wex : 1;
            u32 ch1wid : 4;
            u32 ch1pos : 10;
            bool ch1en : 1;
            bool ch1wex : 1;

            volatile_assignment_operators(xc_a_struct, u32);
        } __attribute__((packed, aligned(4)));

        xc_a_struct rxc_a; //0x0c

        xc_a_struct txc_a; //0x10

        struct dreq_a_struct
        {
            u32 rx : 7;
            w32 reserved_7 : 1;
            u32 tx : 7;
            w32 reserved_15 : 1;
            u32 rx_panic : 7;
            w32 reserved_23 : 1;
            u32 tx_panic : 7;
            w32 reserved_31 : 1;

            volatile_assignment_operators(dreq_a_struct, u32);
        } __attribute__((packed, aligned(4)))
        dreq_a; //0x14

        struct inten_a_struct
        {
            bool txw : 1;
            bool rxr : 1;
            bool txerr : 1;
            bool rxerr : 1;
            w32 reserved_4_31 : 28;

            volatile_assignment_operators(inten_a_struct, u32);
        } __attribute__((packed, aligned(4)))
        inten_a; //0x18

        struct intstc_a_struct
        {
            enum class status_and_clear
            {
                no_interrupt_occured = 0,
                interrupt_occured = 1,
                clear_flag = 1,
            };

            status_and_clear txw : 1;
            status_and_clear rxr : 1;
            status_and_clear txerr : 1;
            status_and_clear rxerr : 1;
            u32 reserved_4_31 : 28;

            volatile_assignment_operators(intstc_a_struct, u32);
        } __attribute__((packed, aligned(4)))
        intstc_a; //0x1c

        struct gray_struct
        {
            bool en : 1;
            bool clr : 1;
            bool flush : 1;
            w32 reserved_3 : 1;
            u32 rxlevel : 6;
            u32 flushed : 6;
            u32 rxfifolevel : 6;
            w32 reserved_22_31 : 10;

            volatile_assignment_operators(gray_struct, u32);
        } __attribute__((packed, aligned(4)))
        gray; //0x20
    } __attribute__((packed, aligned(4)));
    static_assert(sizeof(registers) == 0x24, "i2s register map size does not match datasheet.");

    ///
    ///keeps track of which devices are already used, so only
    ///one instance of each can be created.
    ///
    static bool device_used[device_count];

    ///
    ///base addresses of the different i2s device registers.
    ///
    static constexpr registers* registers_base_address[device_count] = {reinterpret_cast<registers*>(0x3f203000)};

    private:
    ///
    ///remembers which i2s device this is.
    ///
    device device_id;

    ///
    ///pointer to the actual registers of this device.
    ///
    volatile registers* the_registers;

    gpio* pcm_clk;
    gpio* pcm_fs;
    gpio* pcm_din;
    gpio* pcm_dout;

    ///
    ///constructor is private, all objects should be created with
    ///the create function, to avoid making multiple instances of
    ///the same i2s device.
    ///
    i2s(device device_id, gpio* pcm_clk, gpio* pcm_fs, gpio* pcm_din, gpio* pcm_dout);

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
    static i2s* create(device device_id);

    bool transmit_required();
    void transmit(i32 sample);
    bool receive_required();
    i32 receive();

    bool transmit_error();
    bool receive_error();
    void clear_transmit_error();
    void clear_receive_error();

    enum class channel
    {
        left,
        right,
    };

    channel pending_transmit_channel();
    channel pending_receive_channel();
};

#pragma GCC diagnostic pop
#pragma once
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wbitfield-enum-conversion"

#include "gpio.h"
#include "type_definitions.h"
#include "volatile_operators.h"

class spi
{
    private:
    static constexpr usize device_count = 1;
    static constexpr usize cs_count = 2;

    public:
    ///represent all available spi devices.
    enum class device
    {
        spi0 = 0,
    };

    enum class cs_enum
    {
        cs0 = 0,
        cs1 = 1,
    };

    private:
    ///available registers for each spi device.
    struct registers
    {
        struct cs_struct
        {
            cs_enum cs : 2;
            enum class cpha_enum : u32
            {
                first_sclk_at_middle_of_data = 0,
                first_sclk_at_beginning_of_data = 1,
            } cpha : 1;
            enum class cpol_enum : u32
            {
                rest_is_low = 0,
                rest_is_high = 1,
            } cpol : 1;
            enum class clear_enum : u32
            {
                no_action = 0,
                clear_tx_fifo = 1,
                clear_rx_fifo = 2,
                cleart_tx_and_rx_fifo = 3,
            } clear : 2;
            enum class cspol_enum : u32
            {
                cs_active_low = 0,
                cs_active_high = 1,
            } cspol : 1;
            bool ta : 1;
            bool dmaen : 1;
            bool intd : 1;
            bool intr : 1;
            bool adcs : 1;
            bool ren : 1;
            enum class len_enum : u32
            {
                spi_master = 0,
                lossi_master = 1,
            } len : 1;
            enum class lmono_enum : u32
            {
                //unused.
            } lmono : 1;
            enum class te_en_enum : u32
            {
                //unused.
            } te_en : 1;
            bool done : 1;
            bool rxd : 1;
            bool txd : 1;
            bool rxr : 1;
            bool rxf : 1;
            cspol_enum cspol0 : 1;
            cspol_enum cspol1 : 1;
            cspol_enum cspol2 : 1;
            bool dma_len : 1;
            enum class len_long_enum : u32
            {
                write_single_byte = 0,
                write_32_bit_word = 1,
            } len_long : 1;
            w32 reserved_26_31 : 6;

            volatile_assignment_operators(cs_struct, u32);
        }  __attribute__((packed, aligned(4)))
        cs; //0x00

        u32 fifo; //0x04

        struct clk_struct
        {
            u32 cdiv : 16;
            w32 reserved_16_31 : 16;

            volatile_assignment_operators(clk_struct, u32);
        }  __attribute__((packed, aligned(4)))
        clk; //0x08

        struct dlen_struct
        {
            u32 len : 16;
            w32 reserved_16_31 : 16;

            volatile_assignment_operators(dlen_struct, u32);
        }  __attribute__((packed, aligned(4)))
        dlen; //0x0c

        struct ltoh_struct
        {
            u32 toh : 4;
            w32 reserved_4_31 : 28;

            volatile_assignment_operators(ltoh_struct, u32);
        }  __attribute__((packed, aligned(4)))
        ltoh; //0x10

        struct dc_struct
        {
            u32 tdreq : 8;
            u32 tpanic : 8;
            u32 rdreq : 8;
            u32 rpanic : 8;

            volatile_assignment_operators(dc_struct, u32);
        }  __attribute__((packed, aligned(4)))
        dc; //0x14
    }  __attribute__((packed, aligned(4)));
    static_assert(sizeof(registers) == 0x18, "spi register map size does not match datasheet.");

    ///
    ///keeps track of which devices are already used, so only
    ///one instance of each can be created.
    ///
    static bool device_used[device_count];

    ///
    ///base addresses of the different spi device registers.
    ///
    static constexpr registers* registers_base_address[device_count] = {reinterpret_cast<registers*>(0x3f204000)};

    private:
    ///
    ///remembers which spi device this is.
    ///
    device device_id;

    ///
    ///pointer to the actual registers of this device.
    ///
    volatile registers* the_registers;

    gpio* mosi;
    gpio* sclk;
    gpio* cs_pins[cs_count];

    ///
    ///constructor is private, all objects should be created with
    ///the create function, to avoid making multiple instances of
    ///the same spi device.
    ///
    spi(device device_id, gpio* mosi, gpio* sclk, gpio* cs_pins[]);

    public:
    ///
    ///destructor.
    ///
    ~spi();

    ///
    ///returns a pointer to a new device and initialises it. the
    ///user should specify which device it should be, by passing
    ///the correct device enum class. if the device is already in use
    ///a nullptr will be returned.
    ///
    static spi* create(device device_id);

    //write an array of 8 bit words to the given chip in a blocking way.
    void write(cs_enum chip, w8* words, u32 word_count);
};

#pragma GCC diagnostic pop
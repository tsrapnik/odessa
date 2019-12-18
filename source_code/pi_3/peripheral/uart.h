#pragma once
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wbitfield-enum-conversion"

#include "device.h"
#include "gpio.h"
#include "type_definitions.h"
#include "volatile_operators.h"

//todo: use reference manual register names everywhere.
class uart
{
    private:
    static constexpr u32 device_count = 1;

    public:
    //represent all available uart devices.
    enum class device
    {
        uart_pl011 = 0
    };

    private:
    struct registers
    {
        struct dr_struct
        {
            u32 data : 8;
            bool32 fe : 1;
            bool32 pe : 1;
            bool32 be : 1;
            bool32 oe : 1;
            w32 reserved_12_31 : 20;

            volatile_assignment_operators(dr_struct, u32);
        } __attribute__((packed, aligned(4)))
        dr; //0x00

        struct rsrecr_struct
        {
            bool32 fe : 1;
            bool32 pe : 1;
            bool32 be : 1;
            bool32 oe : 1;
            w32 reserved_4_31 : 28;

            volatile_assignment_operators(rsrecr_struct, u32);
        } __attribute__((packed, aligned(4)))
        rsrecr; //0x04

        w32 reserved_0x08_0x14[4]; //0x08-0x14

        struct fr_struct
        {
            bool32 cts : 1;
            w32 dsr : 1; //unsupported.
            w32 dcd : 1; //unsupported.
            bool32 busy : 1;
            bool32 rxfe : 1;
            bool32 txff : 1;
            bool32 rxff : 1;
            bool32 txfe : 1;
            w32 ri : 1; //unsupported.
            w32 reserved_9_31 : 23;

            volatile_assignment_operators(fr_struct, u32);
        } __attribute__((packed, aligned(4)))
        fr; //0x18

        w32 reserved_0x1c; //0x1c

        w32 ilpr; //0x20

        struct ibrd_struct
        {
            u32 ibrd : 16;
            w32 reserved_16_31 : 16;

            volatile_assignment_operators(ibrd_struct, u32);
        } __attribute__((packed, aligned(4)))
        ibrd; //0x24

        struct fbrd_struct
        {
            u32 fbrd : 6;
            w32 reserved_6_31 : 26;

            volatile_assignment_operators(fbrd_struct, u32);
        } __attribute__((packed, aligned(4)))
        fbrd; //0x28

        struct lcrh_struct
        {
            bool32 brk : 1;
            bool32 pen : 1;
            bool32 eps : 1;
            bool32 stp2 : 1;
            bool32 fen : 1;
            enum class wlen_enum : u32
            {
                bits_5 = 0,
                bits_6 = 1,
                bits_7 = 2,
                bits_8 = 3,
            } wlen : 2;
            bool32 sps : 1;
            w32 reserved_8_31 : 24;

            volatile_assignment_operators(lcrh_struct, u32);
        } __attribute__((packed, aligned(4)))
        lcrh; //0x2c

        struct cr_struct
        {
            bool32 uarten : 1;
            bool32 siren : 1;
            bool32 sirlp : 1;
            w32 reserved_3_6 : 4;
            bool32 lbe : 1;
            bool32 txe : 1;
            bool32 rxe : 1;
            w32 dtr : 1; //unsupported.
            bool32 rts : 1;
            w32 out1 : 1; //unsupported.
            w32 out2 : 1; //unsupported.
            bool32 rtsen : 1;
            bool32 ctsen : 1;
            w32 reserved_16_32 : 16;

            volatile_assignment_operators(cr_struct, u32);
        } __attribute__((packed, aligned(4)))
        cr; //0x30

        struct ifls_struct
        {
            enum class txiflsel_enum : u32
            {
                fifo_1_8_full = 0,
                fifo_1_4_full = 1,
                fifo_1_2_full = 2,
                fifo_3_4_full = 3,
                fifo_7_8_full = 4,
            } txiflsel : 3;
            enum class rxiflsel_enum : u32
            {
                fifo_1_8_full = 0,
                fifo_1_4_full = 1,
                fifo_1_2_full = 2,
                fifo_3_4_full = 3,
                fifo_7_8_full = 4,
            } rxiflsel : 3;
            w32 txifpsel : 3; //unsupported.
            w32 rxifpsel : 3; //unsupported.
            w32 reserved_12_31 : 20;

            volatile_assignment_operators(ifls_struct, u32);
        } __attribute__((packed, aligned(4)))
        ifls; //0x34

        struct imsc_struct
        {
            w32 rimim : 1; //unsupported.
            bool32 ctsmim : 1;
            w32 dcdmim : 1; //unsupported.
            w32 dsrmim : 1; //unsupported.
            bool32 rxim : 1;
            bool32 txim : 1;
            bool32 rtim : 1;
            bool32 feim : 1;
            bool32 peim : 1;
            bool32 beim : 1;
            bool32 oeim : 1;
            w32 reserved_11_31 : 21;

            volatile_assignment_operators(imsc_struct, u32);
        } __attribute__((packed, aligned(4)))
        imsc; //0x38

        struct ris_struct
        {
            w32 rirmis : 1; //unsupported.
            bool32 ctsrmis : 1;
            w32 dcdrmis : 1; //unsupported.
            w32 dsrrmis : 1; //unsupported.
            bool32 rxris : 1;
            bool32 txris : 1;
            bool32 rtris : 1;
            bool32 feris : 1;
            bool32 peris : 1;
            bool32 beris : 1;
            bool32 oeris : 1;
            w32 reserved_11_31 : 21;

            volatile_assignment_operators(ris_struct, u32);
        } __attribute__((packed, aligned(4)))
        ris; //0x3c

        struct mis_struct
        {
            w32 rimmis : 1; //unsupported.
            bool32 ctsmmis : 1;
            w32 dcdmmis : 1; //unsupported.
            w32 dsrmmis : 1; //unsupported.
            bool32 rxmis : 1;
            bool32 txmis : 1;
            bool32 rtmis : 1;
            bool32 femis : 1;
            bool32 pemis : 1;
            bool32 bemis : 1;
            bool32 oemis : 1;
            w32 reserved_11_31 : 21;

            volatile_assignment_operators(mis_struct, u32);
        } __attribute__((packed, aligned(4)))
        mis; //0x40

        struct icr_struct
        {
            w32 rimic : 1; //unsupported.
            bool32 ctsmic : 1;
            w32 dcdmic : 1; //unsupported.
            w32 dsrmic : 1; //unsupported.
            bool32 rxic : 1;
            bool32 txic : 1;
            bool32 rtic : 1;
            bool32 feic : 1;
            bool32 peic : 1;
            bool32 beic : 1;
            bool32 oeic : 1;
            w32 reserved_11_31 : 21;

            volatile_assignment_operators(icr_struct, u32);
        } __attribute__((packed, aligned(4)))
        icr; //0x44

        struct dmacr_struct
        {
            w32 rxdmae : 1; //unsupported.
            w32 txdmae : 1; //unsupported.
            w32 dmaonerr : 1; //unsupported.
            w32 reserved_3_31 : 29;

            volatile_assignment_operators(dmacr_struct, u32);
        } __attribute__((packed, aligned(4)))
        dmacr; //0x48

        w32 reserved_0x4c_0x7c[13]; //0x4c-0x7c

        struct itcr_struct
        {
            bool32 itcr0 : 1;
            bool32 itcr1 : 1;
            w32 reserved_2_31 : 30;

            volatile_assignment_operators(itcr_struct, u32);
        } __attribute__((packed, aligned(4)))
        itcr; //0x80

        struct itip_struct
        {
            bool32 itip0 : 1;
            w32 reserved_1_2 : 2;
            bool32 itip3 : 1;
            w32 reserved_4_31 : 28;

            volatile_assignment_operators(itip_struct, u32);
        } __attribute__((packed, aligned(4)))
        itip; //0x84

        struct itop_struct
        {
            bool32 itip0 : 1;
            w32 reserved_1_2 : 2;
            bool32 itip3 : 1;
            w32 reserved_4_5 : 2;
            bool32 itip6 : 1;
            bool32 itip7 : 1;
            bool32 itip8 : 1;
            bool32 itip9 : 1;
            bool32 itip10 : 1;
            bool32 itip11 : 1;
            w32 reserved_12_31 : 20;

            volatile_assignment_operators(itop_struct, u32);
        } __attribute__((packed, aligned(4)))
        itop; //0x88

        struct tdr_struct
        {
            u32 tdr10_0 : 11;
            w32 reserved_11_31 : 21;

            volatile_assignment_operators(tdr_struct, u32);
        } __attribute__((packed, aligned(4)))
        tdr; //0x8c

    } __attribute__((packed, aligned(4))); //todo: explain packed and aligned are necessary. especially aligned, because otherwise byte access in stead of word access.
    static_assert(sizeof(registers) == 0x90, "uart register map size does not match datasheet.");

    //keeps track of which devices are already used, so only
    //one instance of each can be created.
    static bool device_used[device_count];

    //base addresses of the different uart device registers.
    static constexpr registers* registers_base_address[device_count] = {reinterpret_cast<registers*>(0x3f201000)};

    private:
    //remembers which uart device this is.
    device device_id;

    //pointer to the actual registers of this device.
    volatile registers* the_registers;

    gpio* gpio_32;
    gpio* gpio_33;
    gpio* tx_pin;
    gpio* rx_pin;

    //constructor is private, all objects should be created with
    //the create function, to avoid making multiple instances of
    //the same uart device.
    uart(device device_id, gpio* gpio_32, gpio* gpio_33, gpio* tx_pin, gpio* rx_pin);

    public:
    //destructor.
    ~uart();

    //returns a pointer to a new device and initialises it. the
    //user should specify which device it should be, by passing
    //the correct device enum class. if the device is already in use
    //a nullptr will be returned.
    static uart* create(device device_id);

    void write(const char* string);

    //todo: use string class. provide byte array alternative also.
    void write(char* string, u32 size);
};

#pragma GCC diagnostic pop
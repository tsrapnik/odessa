#pragma once

#include "type_definitions.h"

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
        enum class control_and_status_options : u32
        {
            ram_standby = 1 << 25,
            pcm_clock_sync_helper = 1 <<24,
            rx_sign_extend = 1 << 23,
            rx_fifo_is_full = 1 << 22,
            tx_fifo_is_empty = 1 << 21,
            rx_fifo_contains_data = 1 << 20,
            tx_fifo_can_accept_data = 1 << 19,
            rx_fifo_needs_reading = 1 << 18,
            tx_fifo_needs_writing = 1 << 17,
            rx_fifo_error = 1 << 16,
            tx_fifo_error = 1 << 15,
            rx_fifo_in_sync = 1 << 14,
            tx_fifo_in_sync = 1 << 13,
            dma_dreq_enable = 1 << 9,
            rx_fifo_threshold_mask = 0x3 << 7,
            rx_fifo_threshold_single_sample= 0x0 << 7,
            rx_fifo_threshold_fifo_at_least_full = 0x1 << 7,
            rx_fifo_threshold_fifo_at_least = 0x2 << 7,
            rx_fifo_threshold_fifo_full = 0x3 << 7,
            tx_fifo_threshold_mask = 0x3 << 7,
            tx_fifo_threshold_fifo_empty = 0x1 << 5,
            tx_fifo_threshold_fifo_less_than_full = 0x1 << 5,
            // tx_fifo_threshold_fifo_less_than_full = 0x2 << 5,
            tx_fifo_threshold_fifo_full_but_one_sample = 0x3 << 5,
            clear_rx_fifo = 1 << 4,
            clear_tx_fifo = 1 << 3,
            enable_transmission = 1 << 2,
            enable_reception = 1 << 1,
            enable_pcm = 1 << 0,
        };

        control_and_status_options control_and_status; //0x00
        u32 fifo_data; //0x04
        // mode_options mode; //0x08
        // receive_configuration_options receive_configuration; //0x0c
        // transmit_configuration_options transmit_configuration; //0x10
        // dma_request_level_options dma_request_level; //0x14
        // interrupt_enables_options interrupt_enables; //0x18
        // interrupt_status_and_clear_options interrupt_status_and_clear; //0x1c
        // gray_mode_control_options gray_mode_control; //0x20
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
    registers* this_registers;

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

enum test_enum: u32
{
    a = 0,
    b = 1,
    c = 2,
    d = 4,
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
struct test_struct
{
    u32 q: 4;
    test_enum r: 4;
};
#pragma GCC diagnostic pop

//todo:
//is just syntactical sugar.
// struct some_registers
// {
//     u32
//         first_part_of_first_register: 4,
//         second_part_of_first_register: 24,
//         third_part_of_first_register: 4;
//     u32 second_register;
// }

// ..I just realised that the clang flag is -Wbitfield-enum-conversion, not
// -Wenum-bitfield-conversion. Please apply the patch below instead, which
// has replaced the two words to remove the inconsistency.

// 2017-10-16  Sam van Kampen  <sam@segfault.party>

//         * c-family/c.opt: Add a warning flag for struct bit-fields
//         being too small to hold enumerated types.
//         * cp/class.c: Likewise.
//         * doc/invoke.texi: Add documentation for said warning flag.

// Index: gcc/c-family/c.opt
// ===================================================================
// --- gcc/c-family/c.opt	(revision 253784)
// +++ gcc/c-family/c.opt	(working copy)
// @@ -346,6 +346,10 @@ Wframe-address
//  C ObjC C++ ObjC++ Var(warn_frame_address) Warning LangEnabledBy(C ObjC C++ ObjC++,Wall)
//  Warn when __builtin_frame_address or __builtin_return_address is used unsafely.
 
// +Wbitfield-enum-conversion
// +C++ Var(warn_bitfield_enum_conversion) Init(1) Warning
// +Warn about struct bit-fields being too small to hold enumerated types.
// +
//  Wbuiltin-declaration-mismatch
//  C ObjC C++ ObjC++ Var(warn_builtin_declaraion_mismatch) Init(1) Warning
//  Warn when a built-in function is declared with the wrong signature.
// Index: gcc/cp/class.c
// ===================================================================
// --- gcc/cp/class.c	(revision 253784)
// +++ gcc/cp/class.c	(working copy)
// @@ -3278,10 +3278,11 @@ check_bitfield_decl (tree field)
//  		   && tree_int_cst_lt (TYPE_SIZE (type), w)))
//  	warning_at (DECL_SOURCE_LOCATION (field), 0,
//  		    "width of %qD exceeds its type", field);
// -      else if (TREE_CODE (type) == ENUMERAL_TYPE
// +      else if (warn_bitfield_enum_conversion
// +	       && TREE_CODE (type) == ENUMERAL_TYPE
//  	       && (0 > (compare_tree_int
//  			(w, TYPE_PRECISION (ENUM_UNDERLYING_TYPE (type))))))
// -	warning_at (DECL_SOURCE_LOCATION (field), 0,
// +	warning_at (DECL_SOURCE_LOCATION (field), OPT_Wbitfield_enum_conversion,
//  		    "%qD is too small to hold all values of %q#T",
//  		    field, type);
//      }
// Index: gcc/doc/invoke.texi
// ===================================================================
// --- gcc/doc/invoke.texi	(revision 253784)
// +++ gcc/doc/invoke.texi	(working copy)
// @@ -264,7 +264,7 @@ Objective-C and Objective-C++ Dialects}.
//  -Walloca  -Walloca-larger-than=@var{n} @gol
//  -Wno-aggressive-loop-optimizations  -Warray-bounds  -Warray-bounds=@var{n} @gol
//  -Wno-attributes  -Wbool-compare  -Wbool-operation @gol
// --Wno-builtin-declaration-mismatch @gol
// +-Wbitfield-enum-conversion -Wno-builtin-declaration-mismatch @gol
//  -Wno-builtin-macro-redefined  -Wc90-c99-compat  -Wc99-c11-compat @gol
//  -Wc++-compat  -Wc++11-compat  -Wc++14-compat  @gol
//  -Wcast-align  -Wcast-align=strict  -Wcast-qual  @gol
// @@ -5431,6 +5431,12 @@ Incrementing a boolean is invalid in C++17, and de
 
//  This warning is enabled by @option{-Wall}.
 
// +@item -Wbitfield-enum-conversion
// +@opindex Wbitfield-enum-conversion
// +@opindex Wno-bitfield-enum-conversion
// +Warn about a bit-field potentially being too small to hold all values
// +of an enumerated type. This warning is enabled by default.
// +
//  @item -Wduplicated-branches
//  @opindex Wno-duplicated-branches
//  @opindex Wduplicated-branches

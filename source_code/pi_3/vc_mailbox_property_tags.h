#pragma once

#include "type_definitions.h"
#include "enum_flags.h"
#include "vc_pointer.h"

class vc_mailbox_property_tags
{
  public:
    enum class clock_id : u32
    {
        emmc = 0x1,
        uart = 0x2,
        arm = 0x3,
        core = 0x4,
        v3d = 0x5,
        h264 = 0x6,
        isp = 0x7,
        sdram = 0x8,
        pixel = 0x9,
        pwm = 0xa,
    };
    enum class voltage_id : u32
    {
        core = 0x1,
        sdram_c = 0x2,
        sdram_p = 0x3,
        sdram_i = 0x4,
    };
    enum class power_id : u32
    {
        sd_card = 0x0,
        uart0 = 0x1,
        uart1 = 0x2,
        usb_hdc = 0x3,
        i2c0 = 0x4,
        i2c1 = 0x5,
        i2c2 = 0x6,
        spi = 0x7,
        ccp2tx = 0x8,
    };

  private:
    //todo: put tag id's directly in structs with single value enums.
    enum class tag_id : u32
    {
        get_board_model = 0x00010001,
        get_board_revision = 0x00010002,
        get_mac_address = 0x00010003,
        get_board_serial = 0x00010004,
        get_arm_memory = 0x00010005,
        get_vc_memory = 0x00010006,
        get_clocks = 0x00010007,
        get_command_line = 0x00050001,
        get_dma_channels = 0x00060001,
        get_power_state = 0x00020001,
        get_timing = 0x00020002,
        set_power_state = 0x00028001,
        get_clock_state = 0x00030001,
        set_clock_state = 0x00038001,
        get_clock_rate = 0x00030002,
        set_clock_rate = 0x00038002,
        get_max_clock_rate = 0x00030004,
        get_min_clock_rate = 0x00030007,
        get_turbo = 0x00030009,
        set_turbo = 0x00038009,
        get_voltage = 0x00030003,
        set_voltage = 0x00038003,
        get_max_voltage = 0x00030005,
        get_min_voltage = 0x00030008,
        get_temperature = 0x00030006,
        get_max_temperature = 0x0003000a,
        allocate_memory = 0x0003000c,
        lock_memory = 0x0003000d,
        unlock_memory = 0x0003000e,
        release_memory = 0x0003000f,
        execute_code = 0x00030010,
        execute_qpu = 0x00030011,
        enable_qpu = 0x00030012,
        get_dipsmanx_resource_mem_handle = 0x00030014,
        get_edid_block = 0x00030020,
        allocate_buffer = 0x00040001,
        release_buffer = 0x00048001,
        blank_screen = 0x00040002,
        get_physical_display_width_height = 0x00040003,
        test_physical_display_width_height = 0x00044003,
        set_physical_width_height = 0x00048003,
        get_virtual_buffer_width_height = 0x00040004,
        test_virtual_buffer_width_height = 0x00044004,
        set_virtual_buffer_width_height = 0x00048004,
        get_depth = 0x00040005,
        test_depth = 0x00044005,
        set_depth = 0x00048005,
        get_pixel_order = 0x00040006,
        test_pixel_order = 0x00044006,
        set_pixel_order = 0x00048006,
        get_alpha_mode = 0x00040007,
        test_alpha_mode = 0x00044007,
        set_alpha_mode = 0x00048007,
        get_pitch = 0x00040008,
        get_virtual_offset = 0x00040009,
        test_virtual_offset = 0x00044009,
        set_virtal_offset = 0x00048009,
        get_overscan = 0x0004000a,
        test_overscan = 0x0004400a,
        set_overscan = 0x0004800a,
        get_pallette = 0x0004000b,
        test_pallette = 0x0004400b,
        set_pallette = 0x0004800b,
        set_cursor_info = 0x00008010,
        set_cursor_state = 0x00008011,
    };
    enum class request_response_code : u32
    {
        process_request = 0x00000000,
        request_succesful = 0x80000000,
        error_parsing_request = 0x80000001
    };
    struct buffer_header
    {
        const volatile u32 size;
        volatile request_response_code a_request_response_code;
    } __attribute__((packed));
    struct tag_header
    {
        const volatile tag_id a_tag_id;
        const volatile u32 value_buffer_size;
        //initialize both values to zero. is_response is set by vc to 1 to indicate there is a response. response_size is
        //size in bytes of response. never used in practice, as the response size is already known from documentation.
        struct
        {
            //todo: get rid of bitfields.
            u32
                response_size : 31,
                is_response : 1;
        } volatile response_request_code;
    } __attribute__((packed));

  private:
    //static class
    vc_mailbox_property_tags() = delete;
    vc_mailbox_property_tags(const vc_mailbox_property_tags &) = delete;
    vc_mailbox_property_tags &operator=(const vc_mailbox_property_tags &) = delete;
    ~vc_mailbox_property_tags() = delete;

  private:
    struct tag_set_clock_rate
    {
        buffer_header a_buffer_header = {.size = sizeof(tag_set_clock_rate),
                                         .a_request_response_code = request_response_code::process_request};
        tag_header a_tag_header = {.a_tag_id = tag_id::set_clock_rate,
                                   .value_buffer_size = sizeof(tag_set_clock_rate::request),
                                   .response_request_code = {.response_size = 0, .is_response = 0}};
        union {
            struct
            {
                clock_id a_clock_id;
                u32 rate; //in Hz.
                //u32 skip_setting_turbo;
            } request;
            struct
            {
                clock_id a_clock_id;
                u32 rate; //in Hz.
            } response;
        };
        u32 end_tag = 0; //indicates end of the tag.
    } __attribute__((packed, aligned(16)));

  public:
    //set clock with given id to given rate in Hz. returns true on success.
    static bool set_clock_rate(clock_id a_clock_id, u32 rate);

  private:
    struct tag_get_max_clock_rate
    {
        buffer_header a_buffer_header = {.size = sizeof(tag_get_max_clock_rate),
                                         .a_request_response_code = request_response_code::process_request};
        tag_header a_tag_header = {.a_tag_id = tag_id::get_max_clock_rate,
                                   .value_buffer_size = sizeof(tag_get_max_clock_rate::request),
                                   .response_request_code = {.response_size = 0, .is_response = 0}};
        union {
            struct
            {
                clock_id a_clock_id;
            } request;
            struct
            {
                clock_id a_clock_id;
                u32 rate; //in Hz.
            } response;
        };
        u32 end_tag = 0; //indicates end of the tag.
    } __attribute__((packed, aligned(16)));

  public:
    //get maximum frequency in Hz of clock with given id.
    static u32 get_max_clock_rate(clock_id a_clock_id);

  private:
    struct tag_enable_qpu
    {
        buffer_header a_buffer_header = {.size = sizeof(tag_enable_qpu),
                                         .a_request_response_code = request_response_code::process_request};
        tag_header a_tag_header = {.a_tag_id = tag_id::enable_qpu,
                                   .value_buffer_size = sizeof(tag_enable_qpu::request),
                                   .response_request_code = {.response_size = 0, .is_response = 0}};
        union {
            struct
            {
                u32 enable;
            } __attribute__((packed)) request;
            struct
            {
            } __attribute__((packed)) response;
        } __attribute__((packed));
        u32 end_tag = 0; //indicates end of the tag.
    } __attribute__((packed, aligned(16)));

  public:
    //returns true on success.
    static bool enable_qpu(bool enable);

  public:
    enum class allocate_memory_flag
    {
        discardable = 1 << 0,                   //can be resized to 0 at any time. use for cached data.
        normal = 0 << 2,                        //normal allocating alias. don't use from arm.
        direct = 1 << 2,                        //0xc alias uncached.
        coherent = 2 << 2,                      //0x8 alias. non-allocating in l2 but coherent.
        l1_nonallocating = (direct | coherent), //allocating in l2.
        zero = 1 << 4,                          //initialise buffer to all zeros.
        no_init = 1 << 5,                       //don't initialise (default is initialise to all ones).
        hint_permalock = 1 << 6,                //likely to be locked for long periods of time.
    };

  private:
    struct tag_allocate_memory
    {
        buffer_header a_buffer_header = {.size = sizeof(tag_allocate_memory),
                                         .a_request_response_code = request_response_code::process_request};
        tag_header a_tag_header = {.a_tag_id = tag_id::allocate_memory,
                                   .value_buffer_size = sizeof(tag_allocate_memory::request),
                                   .response_request_code = {.response_size = 0, .is_response = 0}};
        union {
            struct
            {
                u32 size;
                u32 alignment;
                allocate_memory_flag flag;
            } request;
            struct
            {
                u32 handle;
            } response;
        };
        u32 end_tag = 0; //indicates end of the tag.
    } __attribute__((packed, aligned(16)));

  public:
    //returns handle on success, returns zero on failure.
    static vc_handle allocate_memory(u32 size, u32 alignment, allocate_memory_flag flag);

  private:
    struct tag_release_memory
    {
        buffer_header a_buffer_header = {.size = sizeof(tag_release_memory),
                                         .a_request_response_code = request_response_code::process_request};
        tag_header a_tag_header = {.a_tag_id = tag_id::release_memory,
                                   .value_buffer_size = sizeof(tag_release_memory::request),
                                   .response_request_code = {.response_size = 0, .is_response = 0}};
        union {
            struct
            {
                u32 handle;
            } request;
            struct
            {
                u32 status;
            } response;
        };
        u32 end_tag = 0; //indicates end of the tag.
    } __attribute__((packed, aligned(16)));

  public:
    //returns true on success.
    static bool release_memory(vc_handle handle);

  private:
    struct tag_lock_memory
    {
        buffer_header a_buffer_header = {.size = sizeof(tag_lock_memory),
                                         .a_request_response_code = request_response_code::process_request};
        tag_header a_tag_header = {.a_tag_id = tag_id::lock_memory,
                                   .value_buffer_size = sizeof(tag_lock_memory::request),
                                   .response_request_code = {.response_size = 0, .is_response = 0}};
        union {
            struct
            {
                u32 handle;
            } request;
            struct
            {
                u32 handle;
            } response;
        };
        u32 end_tag = 0; //indicates end of the tag.
    } __attribute__((packed, aligned(16)));

  public:
    //returns bus address on success, returns zero on failure.
    static vc_pointer lock_memory(vc_handle handle);

  private:
    struct tag_unlock_memory
    {
        buffer_header a_buffer_header = {.size = sizeof(tag_unlock_memory),
                                         .a_request_response_code = request_response_code::process_request};
        tag_header a_tag_header = {.a_tag_id = tag_id::unlock_memory,
                                   .value_buffer_size = sizeof(tag_unlock_memory::request),
                                   .response_request_code = {.response_size = 0, .is_response = 0}};
        union {
            struct
            {
                u32 handle;
            } request;
            struct
            {
                u32 status;
            } response;
        };
        u32 end_tag = 0; //indicates end of the tag.
    } __attribute__((packed, aligned(16)));

  public:
    //returns true on success.
    static bool unlock_memory(vc_handle handle);

  private:
    struct tag_execute_code
    {
        buffer_header a_buffer_header = {.size = sizeof(tag_execute_code),
                                         .a_request_response_code = request_response_code::process_request};
        tag_header a_tag_header = {.a_tag_id = tag_id::execute_code,
                                   .value_buffer_size = sizeof(tag_execute_code::request),
                                   .response_request_code = {.response_size = 0, .is_response = 0}};
        union {
            struct
            {
                u32 function_pointer;
                u32 r0;
                u32 r1;
                u32 r2;
                u32 r3;
                u32 r4;
                u32 r5;
            } request;
            struct
            {
                u32 r0;
            } response;
        };
        u32 handle;
        u32 end_tag = 0; //indicates end of the tag.
    } __attribute__((packed, aligned(16)));

  public:
    //returns true on success.
    static bool execute_code(u32 function_pointer, u32 r0, u32 r1, u32 r2, u32 r3, u32 r4, u32 r5);

  private:
    struct tag_execute_qpu
    {
        buffer_header a_buffer_header = {.size = sizeof(tag_execute_qpu),
                                         .a_request_response_code = request_response_code::process_request};
        tag_header a_tag_header = {.a_tag_id = tag_id::execute_qpu,
                                   .value_buffer_size = sizeof(tag_execute_qpu::request),
                                   .response_request_code = {.response_size = 0, .is_response = 0}};
        union {
            struct
            {
                u32 num_qpus;
                u32 control;
                u32 noflush;
                u32 timeout;
            } request;
            struct
            {
                u32 r0;
            } response;
        };
        u32 end_tag = 0; //indicates end of the tag.
    } __attribute__((packed, aligned(16)));

  public:
    //returns true on success.
    static bool execute_qpu(u32 num_qpus, u32 control, u32 noflush, u32 timeout);
};
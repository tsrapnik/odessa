#pragma once

#include "type_definitions.h"
#include "mutex.h"

class vc_mailbox
{
    public:
    enum class channel : u32
    {
        power_management = 0,
        frame_buffer = 1,
        virtual_uart = 2,
        vchiq = 3,
        leds = 4,
        buttons = 5,
        touch_screen = 6,
        property_tags_arm_to_vc = 8,
        property_tags_vc_to_arm = 9
    };

    private:
    struct mail_box_registers
    {
        volatile u32 read_write; //0x00
        u32 reserved_0x04[3]; //0x04-0x0c
        volatile u32 peek; //0x10
        volatile u32 sender; //0x14
        volatile u32 //0x18
            status_reserved_0 : 30,
            status_empty : 1,
            status_full : 1;
        volatile u32 config; //0x1c
    };

    //mail box 0 is for information from the vc to the arm and should only be read. mail box 1 is for information from the arm to
    //the vc and should only be written.
    static mail_box_registers* const mail_box_0_interface;
    static mail_box_registers* const mail_box_1_interface;

    private:
    static mutex a_mutex;

    private:
    //static class.
    vc_mailbox() = delete;
    vc_mailbox(const vc_mailbox&) = delete;
    vc_mailbox& operator=(const vc_mailbox&) = delete;
    ~vc_mailbox() = delete;

    public:
    static u32 write_read(u32 data, channel a_channel);

    private:
    static void flush();
    static u32 read(channel a_channel);
    static void write(u32 data, channel a_channel);
};
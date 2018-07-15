#pragma once

#include "typeDefinitions.h"
#include "mutex.h"

class mailbox
{
    private:
        mutex this_mutex;
        static mailbox& handle;
    public:
        enum class channel: u32
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
        enum class read_write_options: u32
        {
            channel_mask = 0xf
        };

        enum class peek_options: u32
        {};

        enum class sender_options: u32
        {};

        enum class status_options: u32
        {
            empty = 0x40000000,
            full = 0x80000000
        };

        enum class config_options: u32
        {};

        struct mail_box_registers
        {
            volatile read_write_options read_write; //0x00
            u32 reserved_0x04[ 3 ]; //0x04-0x0c
            volatile peek_options peek;//0x10
            volatile sender_options sender; //0x14
            volatile status_options status; //0x18
            volatile config_options config; //0x1c
        };

        ///
        ///Pointer to array of mailbox interfaces. Only two are available, mail box 0 is for information
        ///from the vc to the arm and should only be read. Mail box 1 is for information from the arm to
        ///the vc and should only be written.
        ///
        static constexpr mail_box_registers* mail_box_interface[ 2 ] = {    reinterpret_cast< mail_box_registers* >( 0x83f000b880 ),
                                                                            reinterpret_cast< mail_box_registers* >( 0x83f000b8a0 ) };

    private:
        ///
        ///Mailbox is singleton to prevent multiple instances accessing the hardware at the same time.
        ///
        mailbox();
        mailbox( const mailbox& ) = delete;
        mailbox& operator=( const mailbox& ) = delete;

    public:
        ~mailbox();

        ///
        ///Returns the reference to the mailbox object. If not yet instantiated it automatically
        ///instantiates it.
        ///
        static mailbox& get_handle();

    public:
        u32 write_read( u32 data, channel this_channel );

    private:
        void flush();
        u32 read( channel this_channel );
        void write( u32 data, channel this_channel );
};
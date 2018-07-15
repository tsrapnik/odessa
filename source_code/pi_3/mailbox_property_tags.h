#pragma once

#include "mailbox.h"

class mailbox_property_tags
{
    public:
        enum request_response_code: u32
        {
            process_request = 0x00000000,
            request_succesful = 0x80000000,
            error_parsing_request = 0x80000001
        };
        struct tag
        {
            u32 identifier;
            u32 buffer_size;
            request_response_code this_request_response_code;
            u8 value_buffer;
            u8 padding;
        };

        struct buffer
        {
            u32 size;
            u32 request_response_code;
            tag tags[ 0 ];
        };
    private:
        mailbox* mailbox_handle;
    
    public:
        /*get_tag(  )
        {
            buffer* this_buffer = static_cast< buffer* >( 0x400000 );
            this_buffer->size = sizeof( buffer ) + sizeof( tag ) + sizeof( u32 );
            this_buffer->this_request_response_code = request_response_code::process_request;
            
        }*/
};


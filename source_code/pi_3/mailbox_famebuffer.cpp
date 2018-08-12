#include "mailbox_framebuffer.h"

mailbox_framebuffer::mailbox_framebuffer():
    this_mailbox( mailbox::get_handle() ),
    this_mailbox_framebuffer_info( new mailbox_framebuffer_info )
{
    this_mailbox_framebuffer_info->width = 640;
    this_mailbox_framebuffer_info->height = 480;
    this_mailbox_framebuffer_info->virtual_width = this_mailbox_framebuffer_info->width;
    this_mailbox_framebuffer_info->virtual_height = this_mailbox_framebuffer_info->height;
    this_mailbox_framebuffer_info->pitch = 0;
    this_mailbox_framebuffer_info->depth = 32;
    this_mailbox_framebuffer_info->offset_x = 0;
    this_mailbox_framebuffer_info->offset_y = 0;
    this_mailbox_framebuffer_info->buffer_pointer = 0;
    this_mailbox_framebuffer_info->buffer_size = 0;

    //clean_data_cache ();
    //data_sync_barrier ();

    this_mailbox.write_read( (u32) (u64) this_mailbox_framebuffer_info, mailbox::channel::frame_buffer );

    //invalidate_data_cache ();

    //data_mem_barrier ();
}

mailbox_framebuffer::~mailbox_framebuffer()
{}
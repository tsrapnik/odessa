#include "vc_mailbox_framebuffer.h"
#include "vc_mailbox.h"
#include "memory.h"

vc_mailbox_framebuffer::vc_mailbox_framebuffer() :
    a_vc_mailbox_framebuffer_info(
        {
            .width = 800,
            .height = 480,
            .virtual_width = 800,
            .virtual_height = 480,
            .pitch = 0,
            .depth = 32,
            .offset_x = 0,
            .offset_y = 0,
            .buffer_pointer = 0,
            .buffer_size = 0,
        }
    )
{
    //todo: pass info to constructor as parameter.

    memory::clean_data_cache();
    memory::data_sync_barrier();
    
    vc_mailbox::write_read(
        vc_pointer::arm_to_vc_pointer(&a_vc_mailbox_framebuffer_info).get_raw_value(),
        vc_mailbox::channel::frame_buffer);

    memory::invalidate_data_cache();
    memory::data_memory_barrier();
}

vc_mailbox_framebuffer::~vc_mailbox_framebuffer()
{
}

vc_pointer vc_mailbox_framebuffer::get_buffer()
{
    return vc_pointer(a_vc_mailbox_framebuffer_info.buffer_pointer);
}

u32 vc_mailbox_framebuffer::get_width()
{
    return a_vc_mailbox_framebuffer_info.width;
}

u32 vc_mailbox_framebuffer::get_height()
{
    return a_vc_mailbox_framebuffer_info.height;
}
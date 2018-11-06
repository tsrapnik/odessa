#include "mailbox_framebuffer.h"
#include "memory.h"

mailbox_framebuffer::mailbox_framebuffer() :
    a_mailbox(mailbox::get_handle()),
    a_mailbox_framebuffer_info()
{
    a_mailbox_framebuffer_info.width = 800;
    a_mailbox_framebuffer_info.height = 480;
    a_mailbox_framebuffer_info.virtual_width = a_mailbox_framebuffer_info.width;
    a_mailbox_framebuffer_info.virtual_height = a_mailbox_framebuffer_info.height;
    a_mailbox_framebuffer_info.pitch = 0;
    a_mailbox_framebuffer_info.depth = 32;
    a_mailbox_framebuffer_info.offset_x = 0;
    a_mailbox_framebuffer_info.offset_y = 0;
    a_mailbox_framebuffer_info.buffer_pointer = 0;
    a_mailbox_framebuffer_info.buffer_size = 0;

    memory::clean_data_cache();
    memory::data_sync_barrier();

    a_mailbox.write_read(mailbox::translate_arm_to_vc(&a_mailbox_framebuffer_info),
                         mailbox::channel::frame_buffer);

    memory::invalidate_data_cache();
    memory::data_memory_barrier();
}

mailbox_framebuffer::~mailbox_framebuffer()
{
}

color* mailbox_framebuffer::get_framebuffer()
{
    return reinterpret_cast<color*>(static_cast<usize>(a_mailbox_framebuffer_info.buffer_pointer & 0x3fffffff));
}

u32 mailbox_framebuffer::get_framebuffer_width()
{
    return a_mailbox_framebuffer_info.width;
}

u32 mailbox_framebuffer::get_framebuffer_height()
{
    return a_mailbox_framebuffer_info.height;
}
#include "mailbox.h"
#include "color.h"

class mailbox_framebuffer
{
    private:
    struct mailbox_framebuffer_info
    {
        u32 width; //physical width in pixels.
        u32 height; //physical height in pixels.
        u32 virtual_width;
        u32 virtual_height;
        u32 pitch; //initialize to 0.
        u32 depth; //bits per pixel.
        u32 offset_x;
        u32 offset_y;
        u32 buffer_pointer; //address of the framebuffer, set by the gpu.
        u32 buffer_size; //size of the framebuffer, set by the gpu.
    } __attribute__((packed, aligned(16)));

    private:
    mailbox& a_mailbox;
    volatile mailbox_framebuffer_info a_mailbox_framebuffer_info;

    public:
    mailbox_framebuffer();
    ~mailbox_framebuffer();
	color* get_framebuffer();
	u32 get_framebuffer_width();
	u32 get_framebuffer_height();
    u64 get_info_address();
};
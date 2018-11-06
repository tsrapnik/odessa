#include "mailbox.h"
#include "color.h"

class mailbox_framebuffer
{
    private:
    struct mailbox_framebuffer_info
    {
        volatile u32 width; //physical width in pixels.
        volatile u32 height; //physical height in pixels.
        volatile u32 virtual_width;
        volatile u32 virtual_height;
        volatile u32 pitch; //initialize to 0.
        volatile u32 depth; //bits per pixel.
        volatile u32 offset_x;
        volatile u32 offset_y;
        volatile u32 buffer_pointer; //address of the framebuffer, set by the gpu.
        volatile u32 buffer_size; //size of the framebuffer, set by the gpu.
    } __attribute__((packed, aligned(16)));

    private:
    mailbox& a_mailbox;
    mailbox_framebuffer_info a_mailbox_framebuffer_info;

    public:
    mailbox_framebuffer();
    ~mailbox_framebuffer();
	color* get_framebuffer();
	u32 get_framebuffer_width();
	u32 get_framebuffer_height();
    u64 get_info_address();
};
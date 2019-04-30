#include "vc_pointer.h"

class vc_mailbox_framebuffer
{
    private:
    struct vc_mailbox_framebuffer_info
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
    vc_mailbox_framebuffer_info a_vc_mailbox_framebuffer_info;

    public:
    vc_mailbox_framebuffer();
    ~vc_mailbox_framebuffer();
	vc_pointer get_buffer();
	u32 get_width();
	u32 get_height();
};
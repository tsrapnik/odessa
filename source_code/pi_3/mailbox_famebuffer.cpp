#include "mailbox_framebuffer.h"

//tmp( move global code to seperate class. )
constexpr u32 l1_data_cache_sets = 128;
constexpr u32 l1_data_cache_ways = 4;
constexpr u32 l1_setway_set_shift = 6;
constexpr u32 l1_setway_way_shift = 30;
constexpr u32 setway_level_shift = 1;
constexpr u32 l2_cache_sets = 512;
constexpr u32 l2_cache_ways = 16;
constexpr u32 l2_setway_way_shift = 28;
constexpr u32 l2_setway_set_shift = 6;

void clean_data_cache( void )
{
	// clean l1 data cache
	for (unsigned n_set = 0; n_set < l1_data_cache_sets; n_set++)
	{
		for (unsigned n_way = 0; n_way < l1_data_cache_ways; n_way++)
		{
			u64 n_set_way_level =   n_way << l1_setway_way_shift
						    | n_set << l1_setway_set_shift
						    | 0 << setway_level_shift;

			asm volatile ("dc csw, %0" : : "r" (n_set_way_level) : "memory");
		}
	}

	// clean l2 unified cache
	for (unsigned n_set = 0; n_set < l2_cache_sets; n_set++)
	{
		for (unsigned n_way = 0; n_way < l2_cache_ways; n_way++)
		{
			u64 n_set_way_level =   n_way << l2_setway_way_shift
						    | n_set << l2_setway_set_shift
						    | 1 << setway_level_shift;

			asm volatile ("dc csw, %0" : : "r" (n_set_way_level) : "memory");
		}
	}
}

void invalidate_data_cache( void )
{
	// invalidate l1 data cache
	for (unsigned n_set = 0; n_set < l1_data_cache_sets; n_set++)
	{
		for (unsigned n_way = 0; n_way < l1_data_cache_ways; n_way++)
		{
			u64 n_set_way_level =   n_way << l1_setway_way_shift
						    | n_set << l1_setway_set_shift
						    | 0 << setway_level_shift;

			asm volatile ("dc isw, %0" : : "r" (n_set_way_level) : "memory");
		}
	}

	// invalidate l2 unified cache
	for (unsigned n_set = 0; n_set < l2_cache_sets; n_set++)
	{
		for (unsigned n_way = 0; n_way < l2_cache_ways; n_way++)
		{
			u64 n_set_way_level =   n_way << l2_setway_way_shift
						    | n_set << l2_setway_set_shift
						    | 1 << setway_level_shift;

			asm volatile ("dc isw, %0" : : "r" (n_set_way_level) : "memory");
		}
	}
}
void data_sync_barrier()
{
    asm volatile ("dsb sy" ::: "memory");
}
void data_mem_barrier()
{
    asm volatile ("dmb sy" ::: "memory");
}

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

    clean_data_cache ();
    data_sync_barrier ();

    this_mailbox.write_read( (u32) (u64) this_mailbox_framebuffer_info, mailbox::channel::frame_buffer );

    invalidate_data_cache ();

    data_mem_barrier ();
}

mailbox_framebuffer::~mailbox_framebuffer()
{}
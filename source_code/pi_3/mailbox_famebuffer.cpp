#include "mailbox_framebuffer.h"

//tmp( Move global code to seperate class. )
constexpr u32 l1_data_cache_sets = 128;
constexpr u32 l1_data_cache_ways = 4;
constexpr u32 l1_setway_set_shift = 6;
constexpr u32 l1_setway_way_shift = 30;
constexpr u32 setway_level_shift = 1;
constexpr u32 l2_cache_sets = 512;
constexpr u32 l2_cache_ways = 16;
constexpr u32 l2_setway_way_shift = 28;
constexpr u32 l2_setway_set_shift = 6;

void CleanDataCache( void )
{
	// clean L1 data cache
	for (unsigned nSet = 0; nSet < l1_data_cache_sets; nSet++)
	{
		for (unsigned nWay = 0; nWay < l1_data_cache_ways; nWay++)
		{
			u64 nSetWayLevel =   nWay << l1_setway_way_shift
						    | nSet << l1_setway_set_shift
						    | 0 << setway_level_shift;

			asm volatile ("dc csw, %0" : : "r" (nSetWayLevel) : "memory");
		}
	}

	// clean L2 unified cache
	for (unsigned nSet = 0; nSet < l2_cache_sets; nSet++)
	{
		for (unsigned nWay = 0; nWay < l2_cache_ways; nWay++)
		{
			u64 nSetWayLevel =   nWay << l2_setway_way_shift
						    | nSet << l2_setway_set_shift
						    | 1 << setway_level_shift;

			asm volatile ("dc csw, %0" : : "r" (nSetWayLevel) : "memory");
		}
	}
}

void InvalidateDataCache( void )
{
	// invalidate L1 data cache
	for (unsigned nSet = 0; nSet < l1_data_cache_sets; nSet++)
	{
		for (unsigned nWay = 0; nWay < l1_data_cache_ways; nWay++)
		{
			u64 nSetWayLevel =   nWay << l1_setway_way_shift
						    | nSet << l1_setway_set_shift
						    | 0 << setway_level_shift;

			asm volatile ("dc isw, %0" : : "r" (nSetWayLevel) : "memory");
		}
	}

	// invalidate L2 unified cache
	for (unsigned nSet = 0; nSet < l2_cache_sets; nSet++)
	{
		for (unsigned nWay = 0; nWay < l2_cache_ways; nWay++)
		{
			u64 nSetWayLevel =   nWay << l2_setway_way_shift
						    | nSet << l2_setway_set_shift
						    | 1 << setway_level_shift;

			asm volatile ("dc isw, %0" : : "r" (nSetWayLevel) : "memory");
		}
	}
}
void DataSyncBarrier()
{
    asm volatile ("dsb sy" ::: "memory");
}
void DataMemBarrier()
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

    CleanDataCache ();
    DataSyncBarrier ();

    this_mailbox.write_read( (u32) (u64) this_mailbox_framebuffer_info, mailbox::channel::frame_buffer );

    InvalidateDataCache ();

    DataMemBarrier ();
}

mailbox_framebuffer::~mailbox_framebuffer()
{}
#include "memory.h"

void memory::clean_data_cache()
{
    clean_l1_data_cache();
    clean_l2_unified_cache();
}

void memory::clean_l1_data_cache()
{
    for(unsigned set = 0; set < l1_data_cache_sets; set++)
    {
        for(unsigned way = 0; way < l1_data_cache_ways; way++)
        {
            u64 set_way_level = way << l1_setway_way_shift |
                                set << l1_setway_set_shift |
                                0 << setway_level_shift;

            asm volatile("dc csw, %0"
                         :
                         : "r"(set_way_level)
                         : "memory");
        }
    }
}

void memory::clean_l2_unified_cache()
{
    for(unsigned set = 0; set < l2_cache_sets; set++)
    {
        for(unsigned way = 0; way < l2_cache_ways; way++)
        {
            u64 set_way_level = way << l2_setway_way_shift |
                                set << l2_setway_set_shift |
                                1 << setway_level_shift;

            asm volatile("dc csw, %0"
                         :
                         : "r"(set_way_level)
                         : "memory");
        }
    }
}

void memory::invalidate_data_cache()
{
    invalidate_l1_data_cache();
    invalidate_l2_unified_cache();
}

void memory::invalidate_l1_data_cache()
{
    for(unsigned set = 0; set < l1_data_cache_sets; set++)
    {
        for(unsigned way = 0; way < l1_data_cache_ways; way++)
        {
            u64 set_way_level = way << l1_setway_way_shift |
                                set << l1_setway_set_shift |
                                0 << setway_level_shift;

            asm volatile("dc isw, %0"
                         :
                         : "r"(set_way_level)
                         : "memory");
        }
    }
}

void memory::invalidate_l2_unified_cache()
{
    for(unsigned set = 0; set < l2_cache_sets; set++)
    {
        for(unsigned way = 0; way < l2_cache_ways; way++)
        {
            u64 set_way_level = way << l2_setway_way_shift |
                                set << l2_setway_set_shift |
                                1 << setway_level_shift;

            asm volatile("dc isw, %0"
                         :
                         : "r"(set_way_level)
                         : "memory");
        }
    }
}

void memory::data_sync_barrier()
{
    asm volatile("dsb sy"
                 :
                 :
                 : "memory");
}

void memory::data_memory_barrier()
{
    asm volatile("dmb sy"
                 :
                 :
                 : "memory");
}
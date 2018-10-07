#pragma once

#include "type_definitions.h"

class memory
{
    public:
    static void clean_data_cache();
    static void clean_l1_data_cache();
    static void clean_l2_unified_cache();
    static void invalidate_data_cache();
    static void invalidate_l1_data_cache();
    static void invalidate_l2_unified_cache();
    static void data_sync_barrier();
    static void data_memory_barrier();

    private:
    static constexpr u32 setway_level_shift = 1;
    static constexpr u32 l1_data_cache_sets = 128;
    static constexpr u32 l1_data_cache_ways = 4;
    static constexpr u32 l1_setway_way_shift = 30; //32 - log2(l1_data_cache_ways)
    static constexpr u32 l1_data_cache_line_length = 64;
    static constexpr u32 l1_setway_set_shift = 6; //log2(l1_data_cache_line_length)
    static constexpr u32 l2_cache_sets = 512;
    static constexpr u32 l2_cache_ways = 16;
    static constexpr u32 l2_setway_way_shift = 28; //32 - log2(l2_cache_ways)
    static constexpr u32 l2_cache_line_length = 64;
    static constexpr u32 l2_setway_set_shift = 6; //log2(l2_cache_line_length)
};
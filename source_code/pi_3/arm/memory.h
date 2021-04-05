#pragma once

#include "type_definitions.h"

class memory
{
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

    public:
    static void clean_data_cache();
    static void clean_l1_data_cache();
    static void clean_l2_unified_cache();
    static void invalidate_data_cache();
    static void invalidate_l1_data_cache();
    static void invalidate_l2_unified_cache();
    static void invalidate_virtual_address_range(void* start, usize size);
    static void data_sync_barrier();
    static void data_memory_barrier();

    private:
    static constexpr u64 attrindx_normal = 0;
    static constexpr u64 attrindx_normal_non_cacheable = 1;
    static constexpr u64 attrindx_device = 2;
    static constexpr u64 sctlr_el1_wxn = (1 << 19);
    static constexpr u64 sctlr_el1_i = (1 << 12);
    static constexpr u64 sctlr_el1_c = (1 << 2);
    static constexpr u64 sctlr_el1_a = (1 << 1);
    static constexpr u64 sctlr_el1_m = (1 << 0);
    static constexpr u64 tcr_el1_ips__shift = 32;
    static constexpr u64 tcr_el1_ips__mask = (7ul << 32);
    static constexpr u64 tcr_el1_ips_4gb = 0ul;
    static constexpr u64 tcr_el1_epd1 = (1 << 23);
    static constexpr u64 tcr_el1_a1 = (1 << 22);
    static constexpr u64 tcr_el1_tg0__shift = 14;
    static constexpr u64 tcr_el1_tg0__mask = (3 << 14);
    static constexpr u64 tcr_el1_tg0_64kb = 1;
    static constexpr u64 tcr_el1_sh0__shift = 12;
    static constexpr u64 tcr_el1_sh0__mask = (3 << 12);
    static constexpr u64 tcr_el1_sh0_inner = 3;
    static constexpr u64 tcr_el1_orgn0__shift = 10;
    static constexpr u64 tcr_el1_orgn0__mask = (3 << 10);
    static constexpr u64 tcr_el1_orgn0_wr_back = 3;
    static constexpr u64 tcr_el1_irgn0__shift = 8;
    static constexpr u64 tcr_el1_irgn0__mask = (3 << 8);
    static constexpr u64 tcr_el1_irgn0_wr_back = 3;
    static constexpr u64 tcr_el1_epd0 = (1 << 7);
    static constexpr u64 tcr_el1_t0sz__shift = 0;
    static constexpr u64 tcr_el1_t0sz__mask = (0x3F << 0);
    static constexpr u64 tcr_el1_t0sz_4gb = 32;
    static constexpr u64 mem_coherent_region = 0x400000;
    static constexpr u64 mem_heap_start = 0x500000;
    static constexpr u64 table_entries = 8192;
    static constexpr u64 ap_table_all_access = 0;
    static constexpr u64 attrib_ap_rw_el1 = 0;
    static constexpr u64 attrib_ap_rw_all = 1;
    static constexpr u64 attrib_ap_ro_el1 = 2;
    static constexpr u64 attrib_ap_ro_all = 3;
    static constexpr u64 attrib_sh_non_shareable = 0;
    static constexpr u64 attrib_sh_outer_shareable = 2;
    static constexpr u64 attrib_sh_inner_shareable = 3;
    static constexpr u64 level_2_block_size = (512 * 1024 * 1024);
    static constexpr u64 level_3_page_size = 0x10000;
    static constexpr u64 page_size = 0x10000;

    struct level_2_table_descriptor
    {
        u64 value_11 : 2, //set to 3.
            ignored_1 : 14, //set to 0.
            table_address : 32, //table base address [47:16].
            reserved_0 : 4, //set to 0.
            ignored_2 : 7, //set to 0.
            pxn_table : 1, //set to 0.
            uxn_table : 1, //set to 0.
            ap_table : 2,
            ns_table : 1; //res0, set to 0.
    }
    __attribute__((packed));

    struct level_2_block_descriptor
    {
        u64 Value01 : 2, //set to 1.
            //lower attributes.
            attribute_index : 3, //[2:0], see mair_el1.
            ns : 1, //res0, set to 0.
            ap : 2, //[2:1].
            sh : 2, //[1:0].
            af : 1, //set to 1, will fault otherwise.
            ng : 1, //set to 0.
            reserved_0_1 : 17, //set to 0.
            output_address : 19, //[47:29].
            reserved_0_2 : 4, //set to 0.
            //upper attributes.
            continuous : 1, //set to 0.
            pxn : 1, //set to 0, 1 for device memory.
            uxn : 1, //set to 1.
            ignored : 9; //set to 0.
    }
    __attribute__((packed));

    struct level_2_invalid_descriptor
    {
        u64 value_0 : 1, //set to 0.
            ignored : 63;
    }
    __attribute__((packed));

    union level_2_descriptor {
        level_2_table_descriptor table;
        level_2_block_descriptor block;
        level_2_invalid_descriptor invalid;
    }
    __attribute__((packed));

    struct level_3_page_descriptor
    {
        u64 value_11 : 2, //set to 3.
            //lower attributes.
            attribute_index : 3, //[2:0], see mair_el1.
            ns : 1, //res0, set to 0.
            ap : 2, //[2:1].
            sh : 2, //[1:0].
            af : 1, //set to 1, will fault otherwise.
            ng : 1, //set to 0.
            reserved_0_1 : 4, //set to 0.
            output_address : 32, //[47:16].
            reserved_0_2 : 4, //set to 0.
            //upper attributes.
            continuous : 1, //set to 0.
            pxn : 1, //set to 0, 1 for device memory.
            uxn : 1, //set to 1.
            ignored : 9 //set to 0.
            ;
    }
    __attribute__((packed));

    struct level_3_invalid_descriptor
    {
        u64 value_0 : 1, //set to 0.
            ignored : 63;
    }
    __attribute__((packed));

    union level_3_descriptor {
        level_3_page_descriptor Page;
        level_3_invalid_descriptor invalid;
    }
    __attribute__((packed));

    private:
    static level_2_descriptor* table;
    static level_3_descriptor* create_level_3_table(u64 base_address, u64 vc_memory_start);
    static level_2_descriptor* create_level_2_table(u64 vc_memory_start);

    public:
    static void enable_mmu();
};
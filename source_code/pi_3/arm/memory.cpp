#include "memory.h"

memory::level_2_descriptor* memory::table;

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

void memory::invalidate_virtual_address_range(void* start, usize size)
{
    u8* address = reinterpret_cast<u8*>(start);
    for(u32 index = 0; index < size; size += 64)
    {
        asm volatile("dc ivac, %0"
                     :
                     : "r"(address)
                     : "memory");
        address += 64;
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

void memory::enable_mmu()
{
    table = create_level_2_table();

    u64 mair_el1 = 0xcc << attrindx_normal * 8 //inner/outer write-back non-transient, non-allocating.
                   | 0x04 << attrindx_device * 8 //device-nGnRE.
                   | 0x00 << attrindx_coherent * 8; //device-nGnRnE.
    asm volatile("msr mair_el1, %0"
                 :
                 : "r"(mair_el1));

    asm volatile("msr ttbr0_el1, %0"
                 :
                 : "r"((u64)(table)));

    u64 tcr_el1;
    asm volatile("mrs %0, tcr_el1"
                 : "=r"(tcr_el1));
    tcr_el1 &= ~(tcr_el1_ips__mask | tcr_el1_a1 | tcr_el1_tg0__mask | tcr_el1_sh0__mask | tcr_el1_orgn0__mask | tcr_el1_irgn0__mask | tcr_el1_epd0 | tcr_el1_t0sz__mask);
    tcr_el1 |= tcr_el1_epd1 | tcr_el1_ips_4gb << tcr_el1_ips__shift | tcr_el1_tg0_64kb << tcr_el1_tg0__shift | tcr_el1_sh0_inner << tcr_el1_sh0__shift | tcr_el1_orgn0_wr_back << tcr_el1_orgn0__shift | tcr_el1_irgn0_wr_back << tcr_el1_irgn0__shift | tcr_el1_t0sz_4gb << tcr_el1_t0sz__shift;
    asm volatile("msr tcr_el1, %0"
                 :
                 : "r"(tcr_el1));

    u32 nsctlr_el1;
    asm volatile("mrs %0, sctlr_el1"
                 : "=r"(nsctlr_el1));
    nsctlr_el1 &= ~(sctlr_el1_wxn | sctlr_el1_a);
    nsctlr_el1 |= sctlr_el1_i | sctlr_el1_c | sctlr_el1_m;
    asm volatile("msr sctlr_el1, %0"
                 :
                 : "r"(nsctlr_el1)
                 : "memory");
}

//todo: change camel to snake case.
void* memset(void* pBuffer, u32 nValue, u64 nLength)
{
    char* p = (char*)pBuffer;

    while(nLength--)
    {
        *p++ = (char)nValue;
    }

    return pBuffer;
}

void* palloc(void)
{
    static u8* s_pNextPage = (u8*)0x1b010000;
    void* pFreePage = (void*)s_pNextPage;
    s_pNextPage += 0x10000;
    return pFreePage;
}

memory::level_3_descriptor* __attribute__((optimize(0))) memory::create_level_3_table(u64 base_address)
{
    level_3_descriptor* level_3_table = (level_3_descriptor*)palloc();

    for(u32 page = 0; page < table_entries; page++) //8192 entries a 64KB
    {
        level_3_page_descriptor* descriptor = &level_3_table[page].Page;

        descriptor->value_11 = 3;
        descriptor->attribute_index = attrindx_normal;
        descriptor->ns = 0;
        descriptor->ap = attrib_ap_rw_el1;
        descriptor->sh = attrib_sh_inner_shareable;
        descriptor->af = 1;
        descriptor->ng = 0;
        descriptor->reserved_0_1 = 0;
        descriptor->output_address = ((base_address) >> 16) & 0xFFFFFFFF;
        descriptor->reserved_0_2 = 0;
        descriptor->continuous = 0;
        descriptor->pxn = 0;
        descriptor->uxn = 1;
        descriptor->ignored = 0;

        u8 _etext;
        if(base_address >= reinterpret_cast<u64>(&_etext))
        {
            descriptor->pxn = 1;

            if(base_address >= 0)
            {
                descriptor->attribute_index = attrindx_device;
                descriptor->sh = attrib_sh_outer_shareable;
            }
            else if(base_address >= mem_coherent_region && base_address < mem_heap_start)
            {
                descriptor->attribute_index = attrindx_coherent;
                descriptor->sh = attrib_sh_outer_shareable;
            }
        }

        base_address += level_3_page_size;
    }

    return level_3_table;
}

memory::level_2_descriptor* __attribute__((optimize(0))) memory::create_level_2_table()
{
    level_2_descriptor* level_2_table;
    level_2_table = reinterpret_cast<level_2_descriptor*>(palloc());

    memset(level_2_table, 0, page_size);

    for(unsigned nEntry = 0; nEntry < 3; nEntry++) //3 entries a 512MB
    {
        u64 base_address = (u64)nEntry * table_entries * level_3_page_size;

        level_3_descriptor* level_3_table = create_level_3_table(base_address);

        level_2_table_descriptor* descriptor = &level_2_table[nEntry].table;

        descriptor->value_11 = 3;
        descriptor->ignored_1 = 0;
        descriptor->table_address = ((reinterpret_cast<u64>(level_3_table) >> 16) & 0xFFFFFFFF);
        descriptor->reserved_0 = 0;
        descriptor->ignored_2 = 0;
        descriptor->pxn_table = 0;
        descriptor->uxn_table = 0;
        descriptor->ap_table = ap_table_all_access;
        descriptor->ns_table = 0;
    }

    data_sync_barrier();

    return level_2_table;
}
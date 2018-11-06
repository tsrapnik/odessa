#pragma once

#include "type_definitions.h"

extern void* __heap_start;

static void* const my_heap_base = reinterpret_cast<void*>(0x500000); //__heap_start; //the first address of the heap.
static const u32 heap_max_order = 12; //the heap size is 2^heap_max_order, this is also the size of the biggest block that can be assigned.
static const u32 heap_min_order = 4; //the smallest memory block that can be assigned is 2^heap_min_order.
static const u32 order_count = heap_max_order - heap_min_order + 1; //the amount of different memory block sizes available, from heap_min_order to (including) heap_max_order.
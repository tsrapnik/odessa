#pragma once

#include "type_definitions.h"

extern u8 __heap_start;

static constexpr u32 heap_max_order = 15; //the heap size is 2^heap_max_order, this is also the size of the biggest block that can be assigned.
static constexpr u32 heap_min_order = 4; //the smallest memory block that can be assigned is 2^heap_min_order.
static constexpr u32 order_count = heap_max_order - heap_min_order + 1; //the amount of different memory block sizes available, from heap_min_order to (including) heap_max_order.

// static const u32 a = static_cast<u32>(reinterpret_cast<usize>(&__heap_start));
// static const u32 b = a + (1 << heap_max_order) - 1;
// static const u32 c = ~((1 << heap_max_order) - 1);
// static const u32 d = b & c;
// static void* const heap_base = reinterpret_cast<void*>(d);

//set the heap_base to a the correct alignment.
static void* const global_heap_base = reinterpret_cast<void*>(
    (reinterpret_cast<usize>(&__heap_start) + (1l << static_cast<usize>(heap_max_order)) - 1l) &
    ~((1l << static_cast<usize>(heap_max_order)) - 1l));
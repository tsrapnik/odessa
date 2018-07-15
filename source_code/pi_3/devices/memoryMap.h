#pragma once

#include "typeDefinitions.h"

static void* const heapBase = ( void* )0x60000000; //The first address of the heap.
static const u32 heapMaxOrder = 20; //The heap size is 2^heapMaxOrder, this is also the size of the biggest block that can be assigned.
static const u32 heapMinOrder = 4; //The smallest memory block that can be assigned is 2^heapMinOrder.
static const u32 orderCount = heapMaxOrder - heapMinOrder + 1; //The amount of different memory block sizes available, from heapMinOrder to (including) heapMaxOrder.
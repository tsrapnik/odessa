#pragma once

#include "memory_map.h"
#include "type_definitions.h"

//todo: make nonstatic class, for reusability on different memory regions.
class buddy_heap
{
    public:
    static void* heap_base;
    struct block //a memory block of size 2^order.
    {
        usize order; //order of the memory block.
        u8 data[]; //array of usable memory bytes.
    };

    struct block_pointer //points to an available memory block and the previous available memory block_pointer in a linked list.
    {
        block_pointer* previous;
        block* own_block;
    };

    static block_pointer block_pointers[1 << (order_count - 2)]; //memory pool of available block pointers. it provides the maximum amount of block_pointers that can be used at the same time.
    static block_pointer* available_block_pointers[1 << (order_count - 2)]; //array of pointers to the memory pool, used to access available block_pointers.
    static block_pointer** last_available_block_pointer; //points to the last available_block_pointer in the memory pool.

    static block_pointer* free_block_lists[heap_max_order + 1]; //points to the linked list of block_pointers pointing to the free memory blocks for each order.

    static usize get_matching_order(usize size); //return the order number of the memory block that has a sufficient size.
    static bool removed_from_free_block_list(block* this_block, u32 order); //return true and remove associated block_pointer, when the block is in the list of free blocks of the given order.

    buddy_heap(); //the class is static. no objects of this type can be made.
    ~buddy_heap();

    public:
    //must be called before using heap memory.
    static void initialize();

    static void* allocate(usize size);
    static void free(void* pointer);

    //todo: remove.
    static void print();
};

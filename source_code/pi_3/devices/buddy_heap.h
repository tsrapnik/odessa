#pragma once

#include "memory_map.h"
#include "type_definitions.h"

class buddy_heap
{
private:
	struct block //a memory block of size 2^order.
	{
		u32 order; //order of the memory block.
		u8 data[]; //array of usable memory bytes.
	};

	struct block_pointer //points to a available memory block and the previous available memory block_pointer in a linked list.
	{
		block_pointer* previous;
		block* own_block;
	};

	static block_pointer block_pointers[ 1 << ( order_count - 2 ) ]; //memory pool of available block pointers. it contains the maximum amount of block_pointers that can be used at the same time.
	static block_pointer* available_block_pointers[ 1 << ( order_count - 2 ) ]; //array of pointers to the memory pool, used to access free block_pointers.
	
	static block_pointer** free_block_pointer; //points to the last available block_pointer in the memory pool.
	static block_pointer* free_blocks[ heap_max_order + 1 ]; //points to the block_pointer reffering to the last available memory block in the linked list for each order.

	static u32 get_matching_order( u32 size ); //return the order number of the memory block that has a sufficient size.
	static bool is_free( block* this_block, u32 order ); //return true en frees the associated block_pointer when the block is in the list of free blocks of the given order.

	buddy_heap(); //the class is static. no objects of this type can be made.
	~buddy_heap();
public:
	//must be called before using heap memory.
	static void initialize();
	
	static void* allocate( u32 size );
	static void free( void* pointer );
};
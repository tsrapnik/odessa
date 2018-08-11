#include "buddy_heap.h"

buddy_heap::block_pointer buddy_heap::block_pointers[ 1 << ( order_count - 2 ) ];
buddy_heap::block_pointer* buddy_heap::available_block_pointers[ 1 << ( order_count - 2 ) ];
buddy_heap::block_pointer** buddy_heap::free_block_pointer;
buddy_heap::block_pointer* buddy_heap::free_blocks[ heap_max_order + 1 ];

u32 buddy_heap::get_matching_order( u32 size )
{
	 size += 4 - 1; //the first byte in the memory block is lost for storing the order. this is why size + 4 is used.
	u32 zeroes;
	asm( "clz %0, %1" : "=r"( zeroes ) : "r"( size ) );
	return 32 - zeroes;
}

bool buddy_heap::is_free( block* this_block, u32 order )
{
	block_pointer* free_block_iterator = free_blocks[ order ];

	if( free_block_iterator == nullptr )
		return false;

	if( free_block_iterator->own_block == this_block ) //if the block is found remove it from the list and return true.
	{
		free_blocks[ order ] = free_block_iterator->previous; //remove the block_pointer from the list.
		free_block_pointer++; //add the block_pointer back to the memory pool.
		*free_block_pointer = free_block_iterator;
		return true;
	}

	block_pointer* previous_free_block_iterator = free_block_iterator;
	free_block_iterator = free_block_iterator->previous;

	while( free_block_iterator != nullptr ) //iterate through all remaining free blocks of the same order.
	{
		if( free_block_iterator->own_block == this_block ) //if the block is found remove it from the list and return true.
		{
			previous_free_block_iterator->previous = free_block_iterator->previous; //remove the block_pointer from the list.
			free_block_pointer++; //add the block_pointer back to the memory pool.
			*free_block_pointer = free_block_iterator;
			return true;
		}
		free_block_iterator = free_block_iterator->previous;
	}
	return false; //return false if the block was not in the list.
}

buddy_heap::buddy_heap()
{}

buddy_heap::~buddy_heap()
{}

//initialize memory pool for the block_pointers and make one block_pointer which refers to the full heap.
void buddy_heap::initialize()
{
	for( u32 i = 0; i < 1 << ( order_count - 2 ); i++ ) //assign each block_pointer to an available_block_pointer.
	{
		available_block_pointers[ i ] = &block_pointers[ i ];
	}

	free_block_pointer = &available_block_pointers[ 0 ]; //point the free_block_pointer to the first available_block_pointer.

	for( u32 i = 0; i < order_count; i++ ) //all linked lists of free_block_pointers for each order are initially empty.
	{
		free_blocks[ i ] = nullptr;
	}

	block_pointer* full_heap = *free_block_pointer; //take the first block_pointer from the memory pool and make it refer to the full heap.
	free_block_pointer++;
	full_heap->own_block = ( block* )heap_base;
	full_heap->own_block->order = heap_max_order;

	free_blocks[ heap_max_order ] = full_heap; //add it to the linked list of the highest order.
	full_heap->previous = nullptr;
}

void* buddy_heap::allocate( u32 size )
{
	u32 order = get_matching_order( size ); //round the size to the first next power of two, so it matches one of the memory block orders.
	block_pointer* free_block_of_order = free_blocks[ order ];
	
	if( free_block_of_order != nullptr ) //check if there is a free block of this order available.
	{
		free_blocks[ order ] = free_block_of_order->previous; //remove the block_pointer from the list.
		free_block_pointer--; //add the block_pointer back to the memory pool.
		*free_block_pointer = free_block_of_order;

		block* free_block = free_block_of_order->own_block; //get the memory location of the free block.
		return ( void* )free_block->data; //return the pointer to the data of the free memory block.
	}
	else
	{
		u32 higher_order = order + 1;
		while( higher_order <= heap_max_order ) //check all higher orders untill a free block is found.
		{
			block_pointer* free_block_of_order = free_blocks[ higher_order ];
			if( free_block_of_order != nullptr ) //check if there is a free block of this order available.
			{
				free_blocks[ higher_order ] = free_block_of_order->previous; //remove the block_pointer from the list.
				free_block_pointer--; //add the block_pointer back to the memory pool.
				*free_block_pointer = free_block_of_order;

				block* free_block = free_block_of_order->own_block; //get the memory location of the free block.

				higher_order--; //lower the order.

				while( higher_order >= order ) //split blocks until the original order is reached.
				{
					block* buddy_block = ( block* )( ( ( ( usize )free_block - ( usize )heap_base ) ^ ( 1 << higher_order ) ) + ( usize )heap_base ); //find the buddy of the split block and set the order.
					buddy_block->order = higher_order;

					block_pointer* buddy_block_pointer = *free_block_pointer; //take a block_pointer from the memory pool.
					free_block_pointer++;

					buddy_block_pointer->own_block = buddy_block; //refer it to the buddy_block.

					block_pointer* previous = free_blocks[ higher_order ]; //add the buddy to the free list.
					buddy_block_pointer->previous = previous;
					free_blocks[ higher_order ] = buddy_block_pointer;

					higher_order--;
				}

				return ( void* )free_block->data; //return the pointer to the data of the free memory block.
			}
			higher_order++;
		}
	}
	return nullptr; //return nullptr if allocation failed.
}

void buddy_heap::free( void* pointer )
{
	block* freed_block = ( block* )( ( u32* )pointer - 1 ); //the pointer address is actually pointing to the data part of the block the block address is one u32 before this address.
	u32 order = freed_block->order;

	block* buddy_block = ( block* )( ( ( ( usize )freed_block - ( usize )heap_base ) ^ ( 1 << order ) ) + ( usize )heap_base ); //find the buddy of the block.

	while( is_free( buddy_block, order ) ) //every time the buddy_block is found to be free, increase the order and check again for the buddy block.
	{
		freed_block = ( block* )( ( ( ( usize )freed_block - ( usize )heap_base ) & ~( 1 << order ) ) + ( usize )heap_base ); //find the first of the free_block and the buddy_block.
		order++; //increase the order
		buddy_block = ( block* )( ( ( ( usize )freed_block - ( usize )heap_base ) ^ ( 1 << order ) ) + ( usize )heap_base ); //find the buddy of the block.
	}

	block_pointer* freed_block_pointer = *free_block_pointer; //take a block_pointer from the memory pool.
	free_block_pointer++;

	freed_block_pointer->own_block = freed_block; //refer it to the freed_block and set the correct order.
	freed_block->order = order;

	block_pointer* previous = free_blocks[ order ]; //add the free_block to the free list.
	freed_block_pointer->previous = previous;
	free_blocks[ order ] = freed_block_pointer;
}

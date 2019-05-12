#include "buddy_heap.h"

buddy_heap::block_pointer buddy_heap::block_pointers[ 1 << ( order_count - 2 ) ];
buddy_heap::block_pointer* buddy_heap::available_block_pointers[ 1 << ( order_count - 2 ) ];
buddy_heap::block_pointer** buddy_heap::last_available_block_pointer;
buddy_heap::block_pointer* buddy_heap::free_block_lists[ heap_max_order + 1 ];
void* buddy_heap::heap_base;

usize buddy_heap::get_matching_order( usize size )
{
	usize word_size = sizeof(usize) * 8; //todo: make word_size somewhere else defined.
	size += word_size / 8 - 1; //the first byte in the memory block is lost for storing the order. this is why word_size in bytes is added.
	usize zeroes;
    asm("clz %0, %1" //todo: move to arm specific static class.
        : "=r"(zeroes)
        : "r"(size));
	return word_size - zeroes;
}

bool buddy_heap::is_free( block* this_block, usize order )
{
	block_pointer* free_block_iterator = free_block_lists[ order ];

	if( free_block_iterator == nullptr )
		return false;

	if( free_block_iterator->own_block == this_block ) //if the block is found remove it from the list and return true.
	{
		free_block_lists[ order ] = free_block_iterator->previous; //remove the block_pointer from the list.
		last_available_block_pointer--; //add the block_pointer back to the memory pool.
		*last_available_block_pointer = free_block_iterator;
		return true;
	}

	block_pointer* previous_free_block_iterator = free_block_iterator;
	free_block_iterator = free_block_iterator->previous;

	while( free_block_iterator != nullptr ) //iterate through all remaining free blocks of the same order.
	{
		if( free_block_iterator->own_block == this_block ) //if the block is found remove it from the list and return true.
		{
			previous_free_block_iterator->previous = free_block_iterator->previous; //remove the block_pointer from the list.
			last_available_block_pointer--; //add the block_pointer back to the memory pool.
			*last_available_block_pointer = free_block_iterator;
			return true;
		}
		free_block_iterator = free_block_iterator->previous;
	}
	return false; //return false if the block was not in the list.
}

buddy_heap::buddy_heap()
{
}

buddy_heap::~buddy_heap()
{
}

//initialize memory pool for the block_pointers and make one block_pointer which refers to the full heap.
void buddy_heap::initialize()
{
	//todo: make class non static. move initialize function to construction and make dependent on parameters in stead of global variables.
	buddy_heap::heap_base = heap_base;
	for( usize i = 0; i < 1 << ( order_count - 2 ); i++ ) //assign each block_pointer to an available_block_pointer.
	{
		available_block_pointers[ i ] = &block_pointers[ i ];
	}

	last_available_block_pointer = &available_block_pointers[ 0 ]; //point the free_block_pointer to the first available_block_pointer.

	for( usize i = 0; i < order_count; i++ ) //all linked lists of free_block_pointers for each order are initially empty.
	{
		free_block_lists[ i ] = nullptr;
	}

	block_pointer* full_heap = *last_available_block_pointer; //take the first block_pointer from the memory pool and make it refer to the full heap.
	last_available_block_pointer++;
	full_heap->own_block = ( block* )buddy_heap::heap_base;
	full_heap->own_block->order = heap_max_order;

	free_block_lists[ heap_max_order ] = full_heap; //add it to the linked list of the highest order.
	full_heap->previous = nullptr;
}

void* buddy_heap::allocate( usize size )
{
	usize order = get_matching_order( size ); //round the size to the first next power of two, so it matches one of the memory block orders.
	if( order < heap_min_order )
		order = heap_min_order;
	block_pointer* free_block_of_order = free_block_lists[ order ];

	if( free_block_of_order != nullptr ) //check if there is a free block of this order available.
	{
		free_block_lists[ order ] = free_block_of_order->previous; //remove the block_pointer from the list.
		last_available_block_pointer--; //add the block_pointer back to the memory pool.
		*last_available_block_pointer = free_block_of_order;

		block* free_block = free_block_of_order->own_block; //get the memory location of the free block.
		return ( void* )free_block->data; //return the pointer to the data of the free memory block.
	}
	else
	{
		usize higher_order = order + 1;
		while( higher_order <= heap_max_order ) //check all higher orders untill a free block is found.
		{
			block_pointer* free_block_of_order = free_block_lists[ higher_order ];
			if( free_block_of_order != nullptr ) //check if there is a free block of this order available.
			{
				free_block_lists[ higher_order ] = free_block_of_order->previous; //remove the block_pointer from the list.
				last_available_block_pointer--; //add the block_pointer back to the memory pool.
				*last_available_block_pointer = free_block_of_order;

				block* free_block = free_block_of_order->own_block; //get the memory location of the free block.

				higher_order--; //lower the order.

				while( higher_order >= order ) //split blocks until the original order is reached.
				{
					block* buddy_block = ( block* )( ( ( ( usize )free_block - ( usize )buddy_heap::heap_base ) ^ ( 1 << higher_order ) ) + ( usize )buddy_heap::heap_base ); //find the buddy of the split block and set the order.
					buddy_block->order = higher_order;

					block_pointer* buddy_block_pointer = *last_available_block_pointer; //take a block_pointer from the memory pool.
					last_available_block_pointer++;

					buddy_block_pointer->own_block = buddy_block; //refer it to the buddy_block.

					block_pointer* previous = free_block_lists[ higher_order ]; //add the buddy to the free list.
					buddy_block_pointer->previous = previous;
					free_block_lists[ higher_order ] = buddy_block_pointer;

					higher_order--;
				}
				free_block->order = order; //set the new order of the block.
				return ( void* )free_block->data; //return the pointer to the data of the free memory block.
			}
			higher_order++;
		}
	}
	return nullptr; //return nullptr if allocation failed.
}

void buddy_heap::free( void* pointer )
{
	//cannot free a nullpointer.
	if( pointer == nullptr )
		return;

	block* freed_block = ( block* )( ( usize* )pointer - 1 ); //the pointer address is actually pointing to the data part of the block the block address is one usize before this address.
	usize order = freed_block->order;

	block* buddy_block = ( block* )( ( ( ( usize )freed_block - ( usize )buddy_heap::heap_base ) ^ ( 1 << order ) ) + ( usize )buddy_heap::heap_base ); //find the buddy of the block.

	while( is_free( buddy_block, order ) ) //every time the buddy_block is found to be free, increase the order and check again for the buddy block.
	{
		freed_block = ( block* )( ( ( ( usize )freed_block - ( usize )buddy_heap::heap_base ) & ~( 1 << order ) ) + ( usize )buddy_heap::heap_base ); //find the first of the free_block and the buddy_block.
		order++; //increase the order
		buddy_block = ( block* )( ( ( ( usize )freed_block - ( usize )buddy_heap::heap_base ) ^ ( 1 << order ) ) + ( usize )buddy_heap::heap_base ); //find the buddy of the block.
	}

	block_pointer* freed_block_pointer = *last_available_block_pointer; //take a block_pointer from the memory pool.
	last_available_block_pointer++;

	freed_block_pointer->own_block = freed_block; //refer it to the freed_block and set the correct order.
	freed_block->order = order;

	freed_block_pointer->previous = free_block_lists[ order ]; //add the free_block to the free list.
	free_block_lists[ order ] = freed_block_pointer;
}
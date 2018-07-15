#include "buddyHeap.h"

buddyHeap::blockPointer buddyHeap::blockPointers[ 1 << ( orderCount - 2 ) ];
buddyHeap::blockPointer* buddyHeap::availableBlockPointers[ 1 << ( orderCount - 2 ) ];
buddyHeap::blockPointer** buddyHeap::freeBlockPointer;
buddyHeap::blockPointer* buddyHeap::freeBlocks[ heapMaxOrder + 1 ];

u32 buddyHeap::getMatchingOrder( u32 size )
{
	 size += 4 - 1; //The first byte in the memory block is lost for storing the order. This is why size + 4 is used.
	u32 zeroes;
	asm( "clz %0, %1" : "=r"( zeroes ) : "r"( size ) );
	return 32 - zeroes;
}

bool buddyHeap::isFree( block* thisBlock, u32 order )
{
	blockPointer* freeBlockIterator = freeBlocks[ order ];

	if( freeBlockIterator == nullptr )
		return false;

	if( freeBlockIterator->ownBlock == thisBlock ) //If the block is found remove it from the list and return true.
	{
		freeBlocks[ order ] = freeBlockIterator->previous; //Remove the blockPointer from the list.
		freeBlockPointer++; //Add the blockPointer back to the memory pool.
		*freeBlockPointer = freeBlockIterator;
		return true;
	}

	blockPointer* previousFreeBlockIterator = freeBlockIterator;
	freeBlockIterator = freeBlockIterator->previous;

	while( freeBlockIterator != nullptr ) //Iterate through all remaining free blocks of the same order.
	{
		if( freeBlockIterator->ownBlock == thisBlock ) //If the block is found remove it from the list and return true.
		{
			previousFreeBlockIterator->previous = freeBlockIterator->previous; //Remove the blockPointer from the list.
			freeBlockPointer++; //Add the blockPointer back to the memory pool.
			*freeBlockPointer = freeBlockIterator;
			return true;
		}
		freeBlockIterator = freeBlockIterator->previous;
	}
	return false; //Return false if the block was not in the list.
}

buddyHeap::buddyHeap()
{}

buddyHeap::~buddyHeap()
{}

//Initialize memory pool for the blockPointers and make one blockPointer which refers to the full heap.
void buddyHeap::initialize()
{
	for( u32 i = 0; i < 1 << ( orderCount - 2 ); i++ ) //Assign each blockPointer to an availableBlockPointer.
	{
		availableBlockPointers[ i ] = &blockPointers[ i ];
	}

	freeBlockPointer = &availableBlockPointers[ 0 ]; //Point the freeBlockPointer to the first availableBlockPointer.

	for( u32 i = 0; i < orderCount; i++ ) //All linked lists of freeBlockPointers for each order are initially empty.
	{
		freeBlocks[ i ] = nullptr;
	}

	blockPointer* fullHeap = *freeBlockPointer; //Take the first blockPointer from the memory pool and make it refer to the full heap.
	freeBlockPointer++;
	fullHeap->ownBlock = ( block* )heapBase;
	fullHeap->ownBlock->order = heapMaxOrder;

	freeBlocks[ heapMaxOrder ] = fullHeap; //Add it to the linked list of the highest order.
	fullHeap->previous = nullptr;
}

void* buddyHeap::allocate( u32 size )
{
	u32 order = getMatchingOrder( size ); //Round the size to the first next power of two, so it matches one of the memory block orders.
	blockPointer* freeBlockOfOrder = freeBlocks[ order ];
	
	if( freeBlockOfOrder != nullptr ) //Check if there is a free block of this order available.
	{
		freeBlocks[ order ] = freeBlockOfOrder->previous; //Remove the blockPointer from the list.
		freeBlockPointer--; //Add the blockPointer back to the memory pool.
		*freeBlockPointer = freeBlockOfOrder;

		block* freeBlock = freeBlockOfOrder->ownBlock; //Get the memory location of the free block.
		return ( void* )freeBlock->data; //Return the pointer to the data of the free memory block.
	}
	else
	{
		u32 higherOrder = order + 1;
		while( higherOrder <= heapMaxOrder ) //Check all higher orders untill a free block is found.
		{
			blockPointer* freeBlockOfOrder = freeBlocks[ higherOrder ];
			if( freeBlockOfOrder != nullptr ) //Check if there is a free block of this order available.
			{
				freeBlocks[ higherOrder ] = freeBlockOfOrder->previous; //Remove the blockPointer from the list.
				freeBlockPointer--; //Add the blockPointer back to the memory pool.
				*freeBlockPointer = freeBlockOfOrder;

				block* freeBlock = freeBlockOfOrder->ownBlock; //Get the memory location of the free block.

				higherOrder--; //Lower the order.

				while( higherOrder >= order ) //Split blocks until the original order is reached.
				{
					block* buddyBlock = ( block* )( ( ( ( usize )freeBlock - ( usize )heapBase ) ^ ( 1 << higherOrder ) ) + ( usize )heapBase ); //Find the buddy of the split block and set the order.
					buddyBlock->order = higherOrder;

					blockPointer* buddyBlockPointer = *freeBlockPointer; //Take a blockPointer from the memory pool.
					freeBlockPointer++;

					buddyBlockPointer->ownBlock = buddyBlock; //Refer it to the buddyBlock.

					blockPointer* previous = freeBlocks[ higherOrder ]; //Add the buddy to the free list.
					buddyBlockPointer->previous = previous;
					freeBlocks[ higherOrder ] = buddyBlockPointer;

					higherOrder--;
				}

				return ( void* )freeBlock->data; //Return the pointer to the data of the free memory block.
			}
			higherOrder++;
		}
	}
	return nullptr; //Return nullptr if allocation failed.
}

void buddyHeap::free( void* pointer )
{
	block* freedBlock = ( block* )( ( u32* )pointer - 1 ); //The pointer address is actually pointing to the data part of the block the block address is one u32 before this address.
	u32 order = freedBlock->order;

	block* buddyBlock = ( block* )( ( ( ( usize )freedBlock - ( usize )heapBase ) ^ ( 1 << order ) ) + ( usize )heapBase ); //Find the buddy of the block.

	while( isFree( buddyBlock, order ) ) //Every time the buddyBlock is found to be free, increase the order and check again for the buddy block.
	{
		freedBlock = ( block* )( ( ( ( usize )freedBlock - ( usize )heapBase ) & ~( 1 << order ) ) + ( usize )heapBase ); //Find the first of the freeBlock and the buddyBlock.
		order++; //Increase the order
		buddyBlock = ( block* )( ( ( ( usize )freedBlock - ( usize )heapBase ) ^ ( 1 << order ) ) + ( usize )heapBase ); //Find the buddy of the block.
	}

	blockPointer* freedBlockPointer = *freeBlockPointer; //Take a blockPointer from the memory pool.
	freeBlockPointer++;

	freedBlockPointer->ownBlock = freedBlock; //Refer it to the freedBlock and set the correct order.
	freedBlock->order = order;

	blockPointer* previous = freeBlocks[ order ]; //Add the freeBlock to the free list.
	freedBlockPointer->previous = previous;
	freeBlocks[ order ] = freedBlockPointer;
}

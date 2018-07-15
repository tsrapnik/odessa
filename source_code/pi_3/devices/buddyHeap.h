#pragma once

#include "memoryMap.h"
#include "typeDefinitions.h"

class buddyHeap
{
private:
	struct block //A memory block of size 2^order.
	{
		u32 order; //Order of the memory block.
		u8 data[]; //Array of usable memory bytes.
	};

	struct blockPointer //Points to a available memory block and the previous available memory blockPointer in a linked list.
	{
		blockPointer* previous;
		block* ownBlock;
	};

	static blockPointer blockPointers[ 1 << ( orderCount - 2 ) ]; //Memory pool of available block pointers. It contains the maximum amount of blockPointers that can be used at the same time.
	static blockPointer* availableBlockPointers[ 1 << ( orderCount - 2 ) ]; //Array of pointers to the memory pool, used to access free blockPointers.
	
	static blockPointer** freeBlockPointer; //Points to the last available blockPointer in the memory pool.
	static blockPointer* freeBlocks[ heapMaxOrder + 1 ]; //Points to the blockPointer reffering to the last available memory block in the linked list for each order.

	static u32 getMatchingOrder( u32 size ); //Return the order number of the memory block that has a sufficient size.
	static bool isFree( block* thisBlock, u32 order ); //Return true en frees the associated blockPointer when the block is in the list of free blocks of the given order.

	buddyHeap(); //The class is static. No objects of this type can be made.
	~buddyHeap();
public:
	//Must be called before using heap memory.
	static void initialize();
	
	static void* allocate( u32 size );
	static void free( void* pointer );
};
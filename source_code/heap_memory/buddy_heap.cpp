#include "buddy_heap.h"

#include "math.h"

buddy_heap::block_pointer buddy_heap::block_pointers[1 << (order_count - 2)];
buddy_heap::block_pointer* buddy_heap::available_block_pointers[1 << (order_count - 2)];
buddy_heap::block_pointer** buddy_heap::last_available_block_pointer;
buddy_heap::block_pointer* buddy_heap::free_block_lists[heap_max_order + 1];
void* buddy_heap::heap_base;

usize buddy_heap::get_matching_order(usize size)
{
    usize word_size = sizeof(usize) * 8;
    size += sizeof(usize) - 1; //the first byte in the memory block is lost for storing the order. this is why word_size in bytes is added.
    return word_size - math::leading_zeroes(size);
}

bool buddy_heap::removed_from_free_block_list(block* this_block, usize order)
{
    block_pointer* free_block_iterator = free_block_lists[order];
    block_pointer* next_free_block_iterator = nullptr;

    while(free_block_iterator != nullptr)
    {
        if(free_block_iterator->own_block == this_block)
        {
            //if found, remove block pointer from the list.
            if(next_free_block_iterator == nullptr)
                free_block_lists[order] = free_block_iterator->previous;
            else
                next_free_block_iterator->previous = free_block_iterator->previous;

            //add block pointer back to the memory pool.
            last_available_block_pointer--;
            *last_available_block_pointer = free_block_iterator;
            return true;
        }
    }
    //return false if the block was not in the list.
    return false;
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
    buddy_heap::heap_base = global_heap_base;
    for(usize i = 0; i < 1 << (order_count - 2); i++) //assign each block_pointer to an available_block_pointer.
    {
        block_pointers[i].previous = nullptr;
        available_block_pointers[i] = &block_pointers[i];
    }

    last_available_block_pointer = &available_block_pointers[0]; //point the last_available_block_pointer to the first available_block_pointer.

    for(usize i = 0; i < heap_max_order + 1; i++) //all linked lists of free_block_lists for each order are initially empty.
    {
        free_block_lists[i] = nullptr;
    }

    block_pointer* full_heap = *last_available_block_pointer; //take the first block_pointer from the memory pool and make it refer to the full heap.
    last_available_block_pointer++;
    full_heap->own_block = reinterpret_cast<block*>(buddy_heap::heap_base);
    full_heap->own_block->order = heap_max_order;

    free_block_lists[heap_max_order] = full_heap; //add it to the linked list of the highest order.
    full_heap->previous = nullptr;
}

void* buddy_heap::allocate(usize size)
{
    usize order = get_matching_order(size); //round the size to the first next power of two, so it matches one of the memory block orders.
    if(order < heap_min_order)
        order = heap_min_order;
    block_pointer* free_block_pointer = free_block_lists[order];

    if(free_block_pointer != nullptr) //check if there is a free block of this order available.
    {
        free_block_lists[order] = free_block_pointer->previous; //remove the block_pointer from the list.
        last_available_block_pointer--; //add the block_pointer back to the memory pool.
        *last_available_block_pointer = free_block_pointer;

        block* free_block = free_block_pointer->own_block; //get the memory location of the free block.
        return reinterpret_cast<void*>(free_block->data); //return the pointer to the data of the free memory block.
    }
    else
    {
        usize higher_order = order + 1;
        while(higher_order <= heap_max_order) //check all higher orders untill a free block is found.
        {
            block_pointer* free_block_pointer = free_block_lists[higher_order];
            if(free_block_pointer != nullptr) //check if there is a free block of this order available.
            {
                free_block_lists[higher_order] = free_block_pointer->previous; //remove the block_pointer from the list.
                last_available_block_pointer--; //add the block_pointer back to the memory pool.
                *last_available_block_pointer = free_block_pointer;

                block* free_block = free_block_pointer->own_block; //get the memory location of the free block.

                higher_order--; //lower the order.

                while(higher_order >= order) //split blocks until the original order is reached.
                {
                    block* buddy_block = reinterpret_cast<block*>(reinterpret_cast<usize>(free_block) ^ (1 << higher_order)); //find the buddy of the split block and set the order.
                    buddy_block->order = higher_order;

                    block_pointer* buddy_block_pointer = *last_available_block_pointer; //take a block_pointer from the memory pool.
                    last_available_block_pointer++;

                    buddy_block_pointer->own_block = buddy_block; //refer it to the buddy_block.

                    buddy_block_pointer->previous = free_block_lists[higher_order]; //add the buddy to the free list.
                    free_block_lists[higher_order] = buddy_block_pointer;

                    higher_order--;
                }
                free_block->order = order; //set the new order of the block.
                return reinterpret_cast<void*>(free_block->data); //return the pointer to the data of the free memory block.
            }
            higher_order++;
        }
    }
    //todo: add error log.
    return nullptr; //return nullptr if allocation failed.
}

void buddy_heap::free(void* pointer)
{
    //cannot free a nullpointer.
    if(pointer == nullptr)
        //todo: add error log.
        return;

    block* freed_block = reinterpret_cast<block*>(reinterpret_cast<usize*>(pointer) - 1); //the pointer address is actually pointing to the data part of the block the block address is one usize before this address.
    usize order = freed_block->order;

    block* buddy_block = reinterpret_cast<block*>(reinterpret_cast<usize>(freed_block) ^ (1 << order)); //find the buddy of the block.

    while(removed_from_free_block_list(buddy_block, order)) //every time the buddy_block is found to be free, increase the order and check again for the buddy block.
    {
        freed_block = reinterpret_cast<block*>(reinterpret_cast<usize>(freed_block) & ~(1 << order)); //find the first of the free_block and the buddy_block.
        order++; //increase the order
        buddy_block = reinterpret_cast<block*>(reinterpret_cast<usize>(freed_block) ^ (1 << order)); //find the buddy of the block.
    }

    block_pointer* freed_block_pointer = *last_available_block_pointer; //take a block_pointer from the memory pool.
    last_available_block_pointer++;

    freed_block_pointer->own_block = freed_block; //refer it to the freed_block and set the correct order.
    freed_block->order = order;

    freed_block_pointer->previous = free_block_lists[order]; //add the free_block to the free list.
    free_block_lists[order] = freed_block_pointer;
}

//todo: remove.
void buddy_heap::print()
{
    extern uart* a_uart;
    char buffer[19];
    u32 available_memory = 0;
    for(u32 order = heap_min_order; order < heap_max_order + 1; order++)
    {
        u32 count = 0;
        block_pointer* a_block_pointer = free_block_lists[order];
        while(a_block_pointer != nullptr)
        {
            count++;
            // a_uart->write(string::to_string(reinterpret_cast<usize>(a_block_pointer), buffer));
            // a_uart->write(".\r\n");
            a_block_pointer = a_block_pointer->previous;
        }
        // a_uart->write("order ");
        // a_uart->write(string::to_string(order, buffer));
        // a_uart->write(": ");
        // a_uart->write(string::to_string(count, buffer));
        // a_uart->write(".\r\n");
        available_memory += count << order;
    }
    a_uart->write(string::to_string(available_memory, buffer));
    a_uart->write(".\r\n");
}
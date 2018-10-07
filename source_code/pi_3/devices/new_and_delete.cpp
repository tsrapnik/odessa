#include "buddy_heap.h"

void* operator new(long unsigned int size)
{
    return buddy_heap::allocate(size);
}
void* operator new[](long unsigned int size)
{
    return buddy_heap::allocate(size);
}
void operator delete(void* pointer)
{
    buddy_heap::free(pointer);
}
void operator delete[](void* pointer)
{
    buddy_heap::free(pointer);
}
void operator delete(void* pointer, long unsigned int size)
{
    buddy_heap::free(pointer);
}
void operator delete[](void* pointer, long unsigned int size)
{
    buddy_heap::free(pointer);
}

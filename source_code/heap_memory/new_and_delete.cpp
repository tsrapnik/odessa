#include "buddy_heap.h"

void* operator new(u64 size)
{
    return buddy_heap::allocate(size);
}

void* operator new[](u64 size)
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

void operator delete(void* pointer, u64 size)
{
    buddy_heap::free(pointer);
}

void operator delete[](void* pointer, u64 size)
{
    buddy_heap::free(pointer);
}

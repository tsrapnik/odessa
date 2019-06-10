#include "buddy_heap.h"

//todo: remove.
#include "string.h"
#include "uart.h"
extern uart* a_uart;
u32 assignment_count = 0;

void* operator new(u64 size)
{
    // assignment_count++;
    // char buffer[19];
    // if(a_uart)
    // {
    //     a_uart->write("new: size:");
    //     a_uart->write(string::to_string(size, buffer), 18);
    //     a_uart->write(", ");
    // }
    void* pointer = buddy_heap::allocate(size);
    // if(a_uart)
    // {
    //     a_uart->write("address: ");
    //     a_uart->write(string::to_string(reinterpret_cast<usize>(pointer) - reinterpret_cast<usize>(buddy_heap::heap_base), buffer), 18);
    //     a_uart->write(".\r\n");
    // }
    return pointer;
}
void* operator new[](u64 size)
{
    // assignment_count++;
    // char buffer[19];
    // a_uart->write("new[]: size:");
    // a_uart->write(string::to_string(size, buffer), 18);
    // a_uart->write(", ");
    void* pointer = buddy_heap::allocate(size);
    // a_uart->write("address: ");
    // a_uart->write(string::to_string(reinterpret_cast<usize>(pointer) - reinterpret_cast<usize>(buddy_heap::heap_base), buffer), 18);
    // a_uart->write(".\r\n");
    return pointer;
}
void operator delete(void* pointer)
{
    // assignment_count--;
    // char buffer[19];
    // a_uart->write("delete: ");
    buddy_heap::free(pointer);
//     a_uart->write("address: ");
//     a_uart->write(string::to_string(reinterpret_cast<usize>(pointer) - reinterpret_cast<usize>(buddy_heap::heap_base), buffer), 18);
//     a_uart->write(".\r\n");
}
void operator delete[](void* pointer)
{
    // assignment_count--;
    // char buffer[19];
    // a_uart->write("delete[]: ");
    buddy_heap::free(pointer);
    // a_uart->write("address: ");
    // a_uart->write(string::to_string(reinterpret_cast<usize>(pointer) - reinterpret_cast<usize>(buddy_heap::heap_base), buffer), 18);
    // a_uart->write(".\r\n");
}
void operator delete(void* pointer, u64 size)
{
    // assignment_count--;
    // char buffer[19];
    // a_uart->write("delete: size: ");
    // a_uart->write(string::to_string(size, buffer), 18);
    buddy_heap::free(pointer);
    // a_uart->write(", address: ");
    // a_uart->write(string::to_string(reinterpret_cast<usize>(pointer) - reinterpret_cast<usize>(buddy_heap::heap_base), buffer), 18);
    // a_uart->write(".\r\n");
}
void operator delete[](void* pointer, u64 size)
{
    // assignment_count--;
    // char buffer[19];
    // a_uart->write("delete: size: ");
    // a_uart->write(string::to_string(size, buffer), 18);
    buddy_heap::free(pointer);
    // a_uart->write(", address: ");
    // a_uart->write(string::to_string(reinterpret_cast<usize>(pointer) - reinterpret_cast<usize>(buddy_heap::heap_base), buffer), 18);
    // a_uart->write(".\r\n");
}

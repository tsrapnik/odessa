#include "debugger.h"

#include "assert.h"

mutex debugger::the_mutex;
uart * debugger::the_uart = nullptr;

void debugger::print(const char* a_string)
{
    the_mutex.lock();

    if(the_uart == nullptr)
    {
        the_uart = uart::create(uart::device::uart_pl011);
        assert(the_uart != nullptr);
    }
    the_uart->write(a_string);

    the_mutex.unlock();
}
#include "uart.h"
#include "mutex.h"

class debugger
{
    private:
    static mutex the_mutex;
    static uart * the_uart;
    public:
    static void print(const char* a_string);
};
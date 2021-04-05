#include "debugger.h"

void assert(bool condition)
{
    //just an infinite loop when the condition is false. this makes it easy to spot where the
    //assertion failed, when debugging.
    if(!condition)
    {
        debugger::print("assertion failed.\r\n");
        while(true)
            ;
    }
}

//todo: move error handlers to pi_3/arm folder?
//error handlers.
extern "C" void err_cur_el0()
{
    debugger::print("err_cur_el0\r\n");
    while(true)
        ;
}
extern "C" void err_cur_elx()
{
    debugger::print("err_cur_elx\r\n");
    while(true)
        ;
}
extern "C" void err_low64_elx()
{
    debugger::print("err_low64_elx\r\n");
    while(true)
        ;
}
extern "C" void err_low32_elx()
{
    debugger::print("err_low32_elx\r\n");
    while(true)
        ;
}
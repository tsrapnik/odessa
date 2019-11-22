#include "scratchpad.h"

template<typename T>
constexpr T max(T a, T b) {
  return (a > b) ? a : b;
}

template<typename T, typename... Args>
constexpr T max(T a, T b, Args... args) {
  return max(max(a,b), args...);
}

interface_0::~interface_0()
{}

void interface_implementor_0::do_stuff()
{
}

void interface_implementor_1::do_stuff()
{
}

void f0(interface_0& a_interface_0)
{
    interface_0 * pointer_to_copy_of_interface_0 = a_interface_0.clone();

    //do stuff with copy.
    pointer_to_copy_of_interface_0->do_stuff();

    delete pointer_to_copy_of_interface_0;
}

void f1(interface_0& a_interface_0)
{
    //weak point in the configuration is setting up the size of the static memory. have not found a decent compile
    //time solution.
    static byte memory [max(sizeof(interface_0), sizeof(interface_implementor_0), sizeof(interface_implementor_1))];

    interface_0 * pointer_to_copy_of_interface_0 = a_interface_0.clone(memory);
    
    //do stuff with copy.
    pointer_to_copy_of_interface_0->do_stuff();

    //no deleting required.
}

void f2()
{
    interface_implementor_0 a0;
    interface_implementor_1 a1;

    f0(a0);
    f0(a1);

    f1(a0);
    f1(a1);
}
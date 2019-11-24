#pragma once

///macro that defines the assignment operators to copy a volatile to non volatile value and vice versa.
///use these inside struct definitions and pass the struct name as actual type. pass as underlying type
///a primitive datatype with the same size as the struct. if the size of the struct does not match a
///primitive datatype, the macro cannot be used.
#define volatile_assignment_operators(actual_type, underlying_type)\
void operator=(const actual_type& rvalue) volatile\
{\
    *reinterpret_cast<volatile underlying_type*>(this) = *reinterpret_cast<const underlying_type*>(&rvalue);\
}\
\
void operator=(const volatile actual_type& rvalue)\
{\
    *reinterpret_cast<underlying_type*>(this) = *reinterpret_cast<const volatile underlying_type*>(&rvalue);\
}

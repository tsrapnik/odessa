#pragma once

#include "type_definitions.h"

class string_
{
    //todo: create full on string class.
    public:
    //todo: change to some kind of constructor, so it is obvious the receiver has to delete the received string afterwards.
    static char* to_string(u64 number);
    static char* to_string(u64 number, char* a_string);
};
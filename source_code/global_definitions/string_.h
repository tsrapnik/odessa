#pragma once

#include "type_definitions.h"

class string_
{
    //naive string implementation. consider improving performance if needed.
    private:
    u32 length;
    char* char_array;

    public:
    enum class integer_style
    {
        hexadecimal,
        decimal,
    };

    string_(const char* literal);
    string_(u64 number, integer_style the_integer_style);
    ~string_();

    char operator[](u32 index);
    operator const char*();

    private:
    static char* create_hexadecimal_string(u64 number, u32& length);
    static char* create_decimal_string(u64 number, u32& length);
    static char digit_to_char(u64 digit);
};
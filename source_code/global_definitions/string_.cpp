#include "string_.h"

string_::string_(const char* literal) :
    length(0),
    char_array(nullptr)
{
    while(literal[length] != '\0')
        length++;
    char_array = new char[length];

    for(u32 index = 0; index < length; index++)
    {
        char_array[index] = literal[index];
    }
}

string_::string_(u64 number, integer_style the_integer_style) :
    length(0),
    char_array(nullptr)
{
    switch(the_integer_style)
    {
        case integer_style::hexadecimal:
            char_array = create_hexadecimal_string(number, length);
            break;

        case integer_style::decimal:
            char_array = create_decimal_string(number, length);
            break;
    }
}

string_::~string_()
{
    delete[] char_array;
}

char string_::operator[](u32 index)
{
    return char_array[index];
}

string_::operator const char*()
{
    return char_array;
}

char* string_::create_hexadecimal_string(u64 number, u32& length)
{
    length = 19;
    char* a_char_array = new char[length];
    a_char_array[0] = '0';
    a_char_array[1] = 'x';
    a_char_array[18] = '\0';
    u64 mask = 0x000000000000000f;

    for(u32 index = 0; index < 16; index++)
    {
        u64 digit = (number >> (4 * (15 - index))) & mask;
        a_char_array[2 + index] = digit_to_char(digit);
    }
    return a_char_array;
}

char* string_::create_decimal_string(u64 number, u32& length)
{
    length = 0;
    char temp_char_array[21];
    temp_char_array[20] = '\0';

    for(u32 index = 0; index < 20; index++)
    {
        u64 digit = number % 10;
        temp_char_array[19 - index] = digit_to_char(digit);
        number /= 10;

        if(digit != 0)
            length = index + 1;
    }

    char* a_char_array;
    if(length == 0)
    {
        a_char_array = new char[2];
        a_char_array[0] = '0';
        a_char_array[1] = '\0';
    }
    else
    {
        a_char_array = new char[length + 1];
        a_char_array[length] = '\0';
        for(u32 index = 0; index < length; index++)
            a_char_array[index] = temp_char_array[20 - length + index];
    }
    
    return a_char_array;
}

char string_::digit_to_char(u64 digit)
{
    switch(digit)
    {
        case 0x0:
            return '0';
            break;
        case 0x1:
            return '1';
            break;
        case 0x2:
            return '2';
            break;
        case 0x3:
            return '3';
            break;
        case 0x4:
            return '4';
            break;
        case 0x5:
            return '5';
            break;
        case 0x6:
            return '6';
            break;
        case 0x7:
            return '7';
            break;
        case 0x8:
            return '8';
            break;
        case 0x9:
            return '9';
            break;
        case 0xa:
            return 'a';
            break;
        case 0xb:
            return 'b';
            break;
        case 0xc:
            return 'c';
            break;
        case 0xd:
            return 'd';
            break;
        case 0xe:
            return 'e';
            break;
        case 0xf:
            return 'f';
            break;
        default:
            return '_';
            break;
    }
}
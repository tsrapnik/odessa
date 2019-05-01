#include "string.h"

<<<<<<< HEAD
//caller of this function has ownership over the u8[].
u8* string::to_string(u64 number)
{
    u8* a_string = new u8[19];
=======
//caller of this function has ownership over the returned char[].
char* string::to_string(u64 number, char* a_string)
{
    // char* a_string = new char[19];
>>>>>>> include_gpu
    a_string[0] = '0';
    a_string[1] = 'x';
    a_string[18] = '\0';
    u64 mask = 0x000000000000000f;

    for(i32 i = 15; i >= 0; i--)
    {
        u64 digit = (number >> (4 * i)) & mask;
        switch(digit)
        {
            case 0x0:
                a_string[2 + 15 - i] = '0';
                break;
            case 0x1:
                a_string[2 + 15 - i] = '1';
                break;
            case 0x2:
                a_string[2 + 15 - i] = '2';
                break;
            case 0x3:
                a_string[2 + 15 - i] = '3';
                break;
            case 0x4:
                a_string[2 + 15 - i] = '4';
                break;
            case 0x5:
                a_string[2 + 15 - i] = '5';
                break;
            case 0x6:
                a_string[2 + 15 - i] = '6';
                break;
            case 0x7:
                a_string[2 + 15 - i] = '7';
                break;
            case 0x8:
                a_string[2 + 15 - i] = '8';
                break;
            case 0x9:
                a_string[2 + 15 - i] = '9';
                break;
            case 0xa:
                a_string[2 + 15 - i] = 'a';
                break;
            case 0xb:
                a_string[2 + 15 - i] = 'b';
                break;
            case 0xc:
                a_string[2 + 15 - i] = 'c';
                break;
            case 0xd:
                a_string[2 + 15 - i] = 'd';
                break;
            case 0xe:
                a_string[2 + 15 - i] = 'e';
                break;
            case 0xf:
                a_string[2 + 15 - i] = 'f';
                break;
            default:
                break;
        }
    }
    return a_string;
}
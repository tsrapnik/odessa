#include "math.h"

f32 math::sqrt(f32 operand)
{
    return 0.0f;
}

f64 math::sqrt(f64 operand)
{
    return 0.0;
}

u32 math::leading_zeroes(u32 operand)
{
    u32 zeroes;
    asm("clz %0, %1"
        : "=r"(zeroes)
        : "r"(operand));
    return zeroes;
}

u32 math::leading_zeroes(u64 operand)
{
    u64 zeroes;
    asm("clz %0, %1"
        : "=r"(zeroes)
        : "r"(operand));
    return zeroes;
}
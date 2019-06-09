#include "math.h"

f32 math::sqrt(f32 operand)
{
    f32 result;
    asm("fsqrt %s[result], %s[operand]"
        : [result]"=w"(result)
        : [operand]"w"(operand));
    return result;
}

f64 math::sqrt(f64 operand)
{
    f64 result;
    asm("fsqrt %d[result], %d[operand]"
        : [result]"=w"(result)
        : [operand]"w"(operand));
    return result;
}

u32 math::leading_zeroes(u32 operand)
{
    u32 zeroes;
    asm("clz %w[zeroes], %w[operand]"
        : [zeroes]"=r"(zeroes)
        : [operand]"r"(operand));
    return zeroes;
}

u64 math::leading_zeroes(u64 operand)
{
    u64 zeroes;
    asm("clz %x[zeroes], %x[operand]"
        : [zeroes]"=r"(zeroes)
        : [operand]"r"(operand));
    return zeroes;
}
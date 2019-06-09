#include "type_definitions.h"

class math
{
    private:
    math();

    public:
    static f32 sqrt(f32 operand);
    static f64 sqrt(f64 operand);
    static u32 leading_zeroes(u32 operand);
    static u64 leading_zeroes(u64 operand);
};
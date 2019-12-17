#pragma once

#include "type_definitions.h"

template <u32 length, typename type>
class vector
{
    public:
    type coordinate[length];
    vector();
    //constructor overload for easy initialization of common 2 and 3 length vector.
    vector(type first, type second);
    vector(type first, type second, type third);
    ~vector();
    static vector sum(const vector& first, const vector& second);
    static vector difference(const vector& first, const vector& second);
    static vector scalar_product(type scalar, const vector& original);
};

template <u32 length, typename type>
vector<length, type>::vector(type first, type second)
{
    coordinate[0] = first;
    coordinate[1] = second;
}

template <u32 length, typename type>
vector<length, type>::vector(type first, type second, type third)
{
    coordinate[0] = first;
    coordinate[1] = second;
    coordinate[2] = third;
}

template <u32 length, typename type>
vector<length, type>::vector()
{
}

template <u32 length, typename type>
vector<length, type>::~vector()
{
}

template <u32 length, typename type>
vector<length, type> vector<length, type>::sum(const vector& first, const vector& second)
{
    vector<length, type> result;
    for(u32 i = 0; i < length; i++)
        result.coordinate[i] = first.coordinate[i] + second.coordinate[i];
    return result;
}

template <u32 length, typename type>
vector<length, type> vector<length, type>::difference(const vector& first, const vector& second)
{
    vector<length, type> result;
    for(u32 i = 0; i < length; i++)
        result.coordinate[i] = first.coordinate[i] - second.coordinate[i];
    return result;
}

template <u32 length, typename type>
vector<length, type> vector<length, type>::scalar_product(type scalar, const vector& original)
{
    vector<length, type> result;
    for(u32 i = 0; i < length; i++)
        result.coordinate[i] = scalar * original.coordinate[i];
    return result;
}

//shorthand names for common combinations.
using vector_2_u32 = vector<2, u32>;
using vector_2_u64 = vector<2, u64>;
using vector_2_f32 = vector<2, f32>;
using vector_2_f64 = vector<2, f64>;
using vector_3_u32 = vector<3, u32>;
using vector_3_u64 = vector<3, u64>;
using vector_3_f32 = vector<3, f32>;
using vector_3_f64 = vector<3, f64>;
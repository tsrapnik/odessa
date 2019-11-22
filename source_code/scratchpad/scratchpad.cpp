#include "scratchpad.h"

template<typename T>
constexpr T max(T a, T b) {
  return (a > b) ? a : b;
}

template<typename T, typename... Args>
constexpr T max(T a, T b, Args... args) {
  return max(max(a,b), args...);
}

i0::~i0()
{}

i0 * m0::clone(byte * memory)
{
    m0 * pm0 = (m0 *)memory;
    *pm0 = *this;
    return pm0;
}

i0 * m0::clone()
{
    m0 * pm0 = new m0;
    *pm0 = *this;
    return pm0;
}

m0::~m0()
{}

void m0::do0()
{
}

m1::~m1()
{}

i0 * m1::clone(byte * memory)
{
    m1 * pm1 = (m1 *)memory;
    *pm1 = *this;
    return pm1;
}

i0 * m1::clone()
{
    m1 * pm1 = new m1;
    *pm1 = *this;
    return pm1;
}

void m1::do0()
{
}

void f0(i0& a_i0)
{
    i0 * m_i0 = a_i0.clone();

    //do stuff with copy.
    m_i0->do0();

    delete m_i0;
}

void f1(i0& a_i0)
{
    static byte memory [max(sizeof(i0), sizeof(m0), sizeof(m1))];

    i0 * m_i0 = a_i0.clone(memory);
    
    //do stuff with copy.
    m_i0->do0();

    //no deleting required.
}

void f2()
{
    m0 a0;
    m1 a1;

    f0(a0);
    f0(a1);

    f1(a0);
    f1(a1);
}
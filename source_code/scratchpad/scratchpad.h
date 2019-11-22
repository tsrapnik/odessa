#pragma once

#include "type_definitions.h"

//file dedicated for testing stuff.

class i0
{
    public:
    virtual ~i0() = 0;
    virtual i0 * clone() = 0;
    virtual i0 * clone(byte * memory) = 0;
    virtual void do0() = 0;
};

class m0 : public i0
{
    public:
    u32 data = 0;
    virtual ~m0();
    virtual i0 * clone() override;
    virtual i0 * clone(byte * memory);
    virtual void do0() override;
};

class m1 : public i0
{
    public:
    u64 data = 0;
    virtual ~m1();
    virtual i0 * clone() override;
    virtual i0 * clone(byte * memory);
    virtual void do0() override;
};
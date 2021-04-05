#pragma once

#include "type_definitions.h"

//file dedicated for testing stuff.

template <typename derived, typename base>
class clone_inherit : public base
{
    public:    
    virtual clone_inherit* clone() const override
    {
        clone_inherit* a_derived = new derived;
        *a_derived = *this;
        return a_derived;
    }

    virtual clone_inherit* clone(w8* memory) const override
    {
        clone_inherit* a_derived = (derived*)memory;
        *a_derived = *this;
        return a_derived;
    }
};

class interface_0
{
    public:
    virtual ~interface_0();
    virtual interface_0* clone() const = 0;
    virtual interface_0* clone(w8* memory) const = 0;
    virtual void do_stuff() = 0;
};

class interface_implementor_0 : public clone_inherit<interface_implementor_0, interface_0>
{
    public:
    u32 data = 0;
    virtual void do_stuff() override;
};

class interface_implementor_1 : public clone_inherit<interface_implementor_1, interface_0>
{
    public:
    u64 data = 0;
    virtual void do_stuff() override;
};
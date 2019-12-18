#pragma once

#include "type_definitions.h"

//should be used as boolean type for register mapped bitfields. using this enum in stead
//of a regular bool ensures that the entire register (on which the bitfield is mapped) gets
//read in stead of just a byte. this is necessary, since some registers can only be accessed
//as 32 bit words.
enum class bool32 : u32 //todo: move to different file if device is deprecated.
{
    false_ = 0,
    true_ = 1,
};

class device //todo: is useless?
{
private:
    //constructor is private, all instances should be created with
    //the create function. this makes it possible to restrict the
    //amount of instances for certain devices.
    device();

public:
    virtual ~device();

    //returns a pointer to a new device and initialises it. if a new
    //device couldn't been created, a nullptr will be returned, so 
    //always check before dereferencing the pointer.
    device* create();
};
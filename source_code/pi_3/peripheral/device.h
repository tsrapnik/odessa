#pragma once

#include "type_definitions.h"

class device
{
private:
    ///
    ///constructor is private, all instances should be created with
    ///the create function. this makes it possible to restrict the
    ///amount of instances for certain devices.
    ///
    device();

public:
    ///
    ///destructor.
    ///
    virtual ~device();

    ///
    ///returns a pointer to a new device and initialises it. if a new
    ///device couldn't been created, a nullptr will be returned, so 
    ///always check before dereferencing the pointer.
    ///
    virtual device* create() = 0;
};
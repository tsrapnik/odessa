#pragma once

#include "typeDefinitions.h"

class device
{
private:
    ///
    ///Constructor is private, all instances should be created with
    ///the create function. This makes it possible to restrict the
    ///amount of instances for certain devices.
    ///
    device();

public:
    ///
    ///Destructor.
    ///
    virtual ~device();

    ///
    ///Returns a pointer to a new device and initialises it. If a new
    ///device couldn't been created, a nullptr will be returned, so 
    ///always check before dereferencing the pointer.
    ///
    virtual device* create() = 0;
};
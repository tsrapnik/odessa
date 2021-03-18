#pragma once

#include "type_definitions.h"

class mutex
{
    private:
        u32 the_lock;
    public:
        mutex();

        void lock();
        void unlock();
};
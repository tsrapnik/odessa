#pragma once

class mutex
{
    private:
        bool locked;
    public:
        mutex();
        
        void lock();
        void unlock();
};
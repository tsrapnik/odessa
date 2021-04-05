#include "mutex.h"

extern "C" void mutex_lock(u32 * the_lock);
extern "C" void mutex_unlock(u32 * the_lock);

mutex::mutex() :
    the_lock(0)
{
}

void mutex::lock()
{
    mutex_lock(&the_lock);
}

void mutex::unlock()
{
    mutex_unlock(&the_lock);
}
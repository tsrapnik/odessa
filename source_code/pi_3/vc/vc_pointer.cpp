#include "vc_pointer.h"

vc_pointer::vc_pointer(u32 raw_value)
{
    this->raw_value = raw_value;
}

void vc_pointer::operator=(const vc_pointer& source) volatile
{
    raw_value = source.raw_value;
}

vc_pointer vc_pointer::arm_to_vc_pointer(void* arm_pointer)
{
    return vc_pointer(static_cast<u32>(reinterpret_cast<usize>(arm_pointer)) | 0xc0000000);
}

vc_pointer vc_pointer::arm_to_vc_pointer(volatile void* arm_pointer)
{
    return arm_to_vc_pointer(const_cast<void*>(arm_pointer));
}

void* vc_pointer::to_arm_pointer() const
{
    return reinterpret_cast<void*>(static_cast<usize>(raw_value & ~0xc0000000));
}

u32 vc_pointer::get_raw_value()
{
    return raw_value;
}
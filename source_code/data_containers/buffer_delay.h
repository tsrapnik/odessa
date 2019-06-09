#pragma once

#include "buffer.h"
class buffer_delay: public buffer {
    private:
    u32 current_position;

    public:
    buffer_delay(u32 max_length);

    ~buffer_delay();

    f32 get_previous(u32 position);
    void update_current(f32 value );
};
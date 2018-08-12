#pragma once

#include "buffer.h"
class buffer_delay: public buffer {
    private:
    int current_position;

    public:
    buffer_delay(int max_length);

    ~buffer_delay();

    double get_previous(int position);
    void update_current(double value );
};
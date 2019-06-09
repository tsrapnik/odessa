#pragma once

#include "effect.h"
#include "list_iterator.h"

enum class looper_state
{
    stopped,
    playing,
    recording,
    overdubbing
};

class effect_looper : public effect
{
    private:
    class channel
    {
        private:
        class track
        {
            private:
            struct block
            {
                f32 frames[256];
            };

            list<block*> blocks;
            list_iterator<block*> block_iterator;
            u32 block_index;

            public:
            track(u32 block_count);

            bool muted;

            void restart_track();

            void to_next_frame();
            f32 get_current_frame();
            void record_next_frame(f32 input_frame, u32* length);
            void update_current_frame(f32 input_frame);
        };

        u32 length;
        list<track*> tracks;
        list_iterator<track*> active_track_pointer;

        public:
        channel();

        f32 play();
        void record(f32 frame);
        void dub(f32 frame);

        void restart();
        void clear();
        void layer_on_active_track();
        void remove_active_track();
    };

    input* mono_input;
    output* mono_output;

    list<channel*> channels;
    list_iterator<channel*> active_channel_pointer;

    looper_state current_state;
    
    void process() override;

    public:
    effect_looper(rectangle bounding_box, color the_color);
    ~effect_looper();
};
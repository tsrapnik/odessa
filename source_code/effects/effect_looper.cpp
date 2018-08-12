#include "effect_looper.h"

effect_looper::effect_looper(rectangle footprint, colour own_colour) :
    effect::effect(footprint, own_colour),
    active_channel_pointer(list_iterator<channel*>(&channels))
{
    mono_input = new input(rectangle(vector_2_int(0, 120), vector_2_int(30, 40)),
                           colour(255, 0, 0, 255));
    mono_output = new output(rectangle(vector_2_int(170, 120), vector_2_int(30, 40)),
                             colour(0, 255, 0, 255));
    add_input(mono_input);
    add_output(mono_output);
}

effect_looper::~effect_looper()
{
}

void effect_looper::process()
{
    float frame = 0.0;
    channel* active_channel = active_channel_pointer.get_data_copy();

    switch(current_state)
    {
        case looper_state::stopped:
        {
            break;
        }
        case looper_state::recording:
        {
            //clear all current tracks of the channel, restart channel and add a first track (and point active_track_pointer to it) before activating this state.
            active_channel->record(mono_input->get_frame());
            break;
        }
        case looper_state::playing:
        {
            //if there are tracks they should not be empty.
            frame += active_channel->play();
            break;
        }
        case looper_state::overdubbing:
        {
            //if there are tracks they should not be empty.
            frame += active_channel->play();
            active_channel->dub(mono_input->get_frame());
            break;
        }
    }
}

effect_looper::channel::channel() :
    active_track_pointer(list_iterator<track*>(&tracks))
{
}

//play all unmuted tracks.
float effect_looper::channel::play()
{
    float frame = 0.0f;

    list_iterator<track*> current_element(&tracks);
    for(current_element.to_first(); !current_element.at_end(); current_element++)
    {
        channel::track* current_track = current_element.get_data_copy();
        current_track->to_next_frame();
        if(!current_track->muted)
        {
            frame += current_track->get_current_frame();
        }
    }

    return frame;
}

//record the input.
void effect_looper::channel::record(float frame)
{
    channel::track* active_track = active_track_pointer.get_data_copy();
    active_track->record_next_frame(frame, &length);
}

//dub active track with the input.
void effect_looper::channel::dub(float frame)
{
    channel::track* active_track = active_track_pointer.get_data_copy();
    active_track->update_current_frame(frame);
}

//sets all tracks to the first frame.
void effect_looper::channel::restart()
{
    list_iterator<track*> current_element(&tracks);
    for(current_element.to_first(); !current_element.at_end(); current_element++)
    {
        track* current_track = current_element.get_data_copy();
        current_track->restart_track();
    }
}

//delete all tracks.
void effect_looper::channel::clear()
{
    tracks.delete_all();
    active_track_pointer.to_first();
    length = 0;
}

//add a new track on top of the active track and make this the new active track.
void effect_looper::channel::layer_on_active_track()
{
    track* new_track = new track(length);
    tracks.append_new(new_track);
}

//delete the active track and set the previous track as the active one.
void effect_looper::channel::remove_active_track()
{
    active_track_pointer.delete_current();
}

//create empty track with "block_count" blocks.
effect_looper::channel::track::track(int block_count) :
    block_iterator(&blocks),
    block_index(0)
{
    for(int i = 0; i < block_count; i++)
    {
        blocks.append_new();
    }
}

//sets track to it's first frame.
void effect_looper::channel::track::restart_track()
{
    block_iterator.to_first();
    block_index = 0;
}

void effect_looper::channel::track::to_next_frame()
{
    block_index++;
    if(block_index > 255) //load next block when current block is finished.
    {
        block_index = 0;
        block_iterator++;
        if(block_iterator.at_end()) //go back to first block when all blocks are traversed.
            block_iterator.to_first();
    }
}

float effect_looper::channel::track::get_current_frame()
{
    track::block* current_block = block_iterator.get_data_copy();
    return current_block->frames[block_index];
}

//fills the next frame with the input_frame-data, creates a new block if necessary.
void effect_looper::channel::track::record_next_frame(float input_frame, int* length)
{
    block_index++;
    if(block_index > 255) //create new block when current block is finished.
    {
        block_index = 0;
        blocks.append_new();
        block_iterator++;
        length++;
    }
    track::block* current_block = block_iterator.get_data_copy(); //fill the next track.
    current_block->frames[block_index] = input_frame;
}

void effect_looper::channel::track::update_current_frame(float input_frame)
{
    track::block* current_block = block_iterator.get_data_copy();
    current_block->frames[block_index] = input_frame;
}

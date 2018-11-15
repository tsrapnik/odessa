#include "mailbox_property_tags.h"

mailbox_property_tags::mailbox_property_tags() :
    a_mailbox(mailbox::get_handle())
{
}

mailbox_property_tags::~mailbox_property_tags()
{
}

mailbox_property_tags& mailbox_property_tags::get_handle()
{
    static mailbox_property_tags handle;
    return handle;
}

bool mailbox_property_tags::set_clock_rate(clock_id a_clock_id, u32 rate)
{
    tag_set_clock_rate a_tag_set_clock_rate;
    a_tag_set_clock_rate.request.a_clock_id = a_clock_id;
    a_tag_set_clock_rate.request.rate = rate;

    a_mailbox.write_read(mailbox::cast_to_vc_pointer(const_cast<tag_set_clock_rate*>(&a_tag_set_clock_rate)),
                         mailbox::channel::property_tags_arm_to_vc);
    if(a_tag_set_clock_rate.a_buffer_header.a_request_response_code == request_response_code::request_succesful)
        return true;
    return false;
}

u32 mailbox_property_tags::get_max_clock_rate(clock_id a_clock_id)
{
    tag_get_max_clock_rate a_tag_get_max_clock_rate;
    a_tag_get_max_clock_rate.request.a_clock_id = a_clock_id;

    a_mailbox.write_read(mailbox::cast_to_vc_pointer(const_cast<tag_get_max_clock_rate*>(&a_tag_get_max_clock_rate)),
                         mailbox::channel::property_tags_arm_to_vc);
    return a_tag_get_max_clock_rate.response.rate;
}

bool mailbox_property_tags::enable_qpu(bool enable)
{
    tag_enable_qpu a_tag_enable_qpu;
    a_tag_enable_qpu.request.enable = 1;

    a_mailbox.write_read(mailbox::cast_to_vc_pointer(const_cast<tag_enable_qpu*>(&a_tag_enable_qpu)),
                         mailbox::channel::property_tags_arm_to_vc);
    if(a_tag_enable_qpu.a_buffer_header.a_request_response_code == request_response_code::request_succesful)
        return true;
    return false;
}

u32 mailbox_property_tags::allocate_memory(u32 size, u32 alignment, allocate_memory_flag flag)
{
    //todo: exception if fails.
    tag_allocate_memory a_tag_allocate_memory;
    a_tag_allocate_memory.request.size = size;
    a_tag_allocate_memory.request.alignment = alignment;
    a_tag_allocate_memory.request.flag = flag;

    a_mailbox.write_read(mailbox::cast_to_vc_pointer(const_cast<tag_allocate_memory*>(&a_tag_allocate_memory)),
                         mailbox::channel::property_tags_arm_to_vc);
    if(a_tag_allocate_memory.a_buffer_header.a_request_response_code == request_response_code::request_succesful)
        return a_tag_allocate_memory.response.handle;
    return 0;
}

bool mailbox_property_tags::release_memory(u32 handle)
{
    tag_release_memory a_tag_release_memory;
    a_tag_release_memory.request.handle = handle;

    a_mailbox.write_read(mailbox::cast_to_vc_pointer(const_cast<tag_release_memory*>(&a_tag_release_memory)),
                         mailbox::channel::property_tags_arm_to_vc);
    if(a_tag_release_memory.a_buffer_header.a_request_response_code == request_response_code::request_succesful)
        if(a_tag_release_memory.response.status == 0)
            return true;
    return false;
}

u32 mailbox_property_tags::lock_memory(u32 handle)
{
    tag_lock_memory a_tag_lock_memory;
    a_tag_lock_memory.request.handle = handle;

    a_mailbox.write_read(mailbox::cast_to_vc_pointer(const_cast<tag_lock_memory*>(&a_tag_lock_memory)),
                         mailbox::channel::property_tags_arm_to_vc);
    if(a_tag_lock_memory.a_buffer_header.a_request_response_code == request_response_code::request_succesful)
        return a_tag_lock_memory.response.bus_address;
    return 0;
}

bool mailbox_property_tags::unlock_memory(u32 handle)
{
    tag_unlock_memory a_tag_unlock_memory;
    a_tag_unlock_memory.request.handle = handle;

    a_mailbox.write_read(mailbox::cast_to_vc_pointer(const_cast<tag_unlock_memory*>(&a_tag_unlock_memory)),
                         mailbox::channel::property_tags_arm_to_vc);
    if(a_tag_unlock_memory.a_buffer_header.a_request_response_code == request_response_code::request_succesful)
        if(a_tag_unlock_memory.response.status == 0)
            return true;
    return false;
}

bool mailbox_property_tags::execute_code(u32 function_pointer, u32 r0, u32 r1, u32 r2, u32 r3, u32 r4, u32 r5)
{
    tag_execute_code a_tag_execute_code;
    a_tag_execute_code.request.function_pointer = function_pointer;
    a_tag_execute_code.request.r0 = r0;
    a_tag_execute_code.request.r1 = r1;
    a_tag_execute_code.request.r2 = r2;
    a_tag_execute_code.request.r3 = r3;
    a_tag_execute_code.request.r4 = r4;
    a_tag_execute_code.request.r5 = r5;

    a_mailbox.write_read(mailbox::cast_to_vc_pointer(const_cast<tag_execute_code*>(&a_tag_execute_code)),
                         mailbox::channel::property_tags_arm_to_vc);
    if(a_tag_execute_code.a_buffer_header.a_request_response_code == request_response_code::request_succesful)
        return true;
    return false;
}

bool mailbox_property_tags::execute_qpu(u32 num_qpus, u32 control, u32 noflush, u32 timeout)
{
    tag_execute_qpu a_tag_execute_qpu;
    a_tag_execute_qpu.request.num_qpus = num_qpus;
    a_tag_execute_qpu.request.control = control;
    a_tag_execute_qpu.request.noflush = noflush;
    a_tag_execute_qpu.request.timeout = timeout;

    a_mailbox.write_read(mailbox::cast_to_vc_pointer(const_cast<tag_execute_qpu*>(&a_tag_execute_qpu)),
                         mailbox::channel::property_tags_arm_to_vc);
    if(a_tag_execute_qpu.a_buffer_header.a_request_response_code == request_response_code::request_succesful)
        return true;
    return a_tag_execute_qpu.a_tag_header.value_buffer_size;
}
#include "mailbox.h"
#include "enum_flags.h"
#include "mutex.h"

mailbox::mail_box_registers* const mailbox::mail_box_0_interface = reinterpret_cast<mail_box_registers*>(0x3f00b880);
mailbox::mail_box_registers* const mailbox::mail_box_1_interface = reinterpret_cast<mail_box_registers*>(0x3f00b8a0);

mailbox::mailbox()
{
}

mailbox::~mailbox()
{
}

mailbox& mailbox::get_handle()
{
    static mailbox handle;
    return handle;
}

u32 mailbox::cast_to_vc_pointer(void* arm_address)
{
    return static_cast<u32>(reinterpret_cast<usize>(arm_address)) | 0xc0000000;
}

void* mailbox::cast_to_arm_pointer(u32 vc_address)
{
    return reinterpret_cast<void*>(static_cast<usize>(vc_address & ~0xc0000000));
}

u32 mailbox::write_read(u32 data, channel a_channel)
{
    a_mutex.lock();

    flush();
    write(data, a_channel);
    data = read(a_channel);

    a_mutex.unlock();
    return data;
}

void mailbox::flush()
{
    while(!mail_box_0_interface->status_empty)
    {
        mail_box_0_interface->read_write;
    }
}

u32 mailbox::read(channel a_channel)
{
    u32 data;
    do
    {
        while(mail_box_0_interface->status_empty)
            ;
        data = mail_box_0_interface->read_write;
    } while(static_cast<channel>(data & 0xf) != a_channel); //first 4 bits are occupied by the channel.
    return data & 0xfffffff0; //last 28 bits contain the data.
}

void mailbox::write(u32 data, channel a_channel)
{
    while(mail_box_1_interface->status_full)
        ;
    mail_box_1_interface->read_write = static_cast<u32>(a_channel) | (data & 0xfffffff0); //first 4 bits are occupied by the channel,last 28 bits contain the data.
}
#include "vc_mailbox.h"
#include "memory.h"

vc_mailbox::mail_box_registers* const vc_mailbox::mail_box_0_interface = reinterpret_cast<mail_box_registers*>(0x3f00b880);
vc_mailbox::mail_box_registers* const vc_mailbox::mail_box_1_interface = reinterpret_cast<mail_box_registers*>(0x3f00b8a0);
mutex vc_mailbox::a_mutex;
u32 vc_mailbox::write_read(u32 data, channel a_channel)
{
    a_mutex.lock();

    flush();
    write(data, a_channel);
    data = read(a_channel);

    a_mutex.unlock();
    return data;
}

void vc_mailbox::flush()
{
    while(!mail_box_0_interface->status_empty)
    {
        mail_box_0_interface->read_write;
    }
}

u32 vc_mailbox::read(channel a_channel)
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

void vc_mailbox::write(u32 data, channel a_channel)
{
    while(mail_box_1_interface->status_full)
        ;
    memory::data_memory_barrier();
    mail_box_1_interface->read_write = static_cast<u32>(a_channel) | (data & 0xfffffff0); //first 4 bits are occupied by the channel,last 28 bits contain the data.
    memory::data_memory_barrier();
}
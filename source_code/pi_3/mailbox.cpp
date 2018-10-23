#include "mailbox.h"
#include "enum_flags.h"
#include "mutex.h"

constexpr mailbox::mail_box_registers* mailbox::mail_box_0_interface;
constexpr mailbox::mail_box_registers* mailbox::mail_box_1_interface;

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

u32 mailbox::write_read(u32 data, channel a_channel)
{
    a_mutex.lock();

    flush();
    write(data, a_channel);
    u32 result = read(a_channel);

    a_mutex.unlock();
    return result;
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
    u32 read_write;
    do
    {
        while(mail_box_0_interface->status_empty)
            ;
        read_write = mail_box_0_interface->read_write;
    } while(static_cast<channel>(read_write & 0xf) != a_channel); //first 4 bits are occupied by the channel.
    return read_write & 0xfffffff0; //last 28 bits contain the data.
}

void mailbox::write(u32 data, channel a_channel)
{
    while(mail_box_1_interface->status_full)
        ;
    mail_box_1_interface->read_write = static_cast<u32>(a_channel) | ((0xc0000000 + data) & 0xfffffff0); //first 4 bits are occupied by the channel,last 28 bits contain the data.
}
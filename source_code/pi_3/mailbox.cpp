#include "mailbox.h"
#include "enum_flags.h"
#include "mutex.h"

extern "C" void delay_loop(int);

constexpr mailbox::mail_box_registers* mailbox::mail_box_interface[2];

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

u32 mailbox::write_read(u32 data, channel this_channel)
{
    this_mutex.lock();

    flush();
    write(data, this_channel);
    u32 result = read(this_channel);

    this_mutex.unlock();
    return result;
}

void mailbox::flush()
{
    while(!(static_cast<bool>(mail_box_interface[0]->status & status_options::empty)))
    {
        mail_box_interface[0]->read_write;
        delay_loop(350000 * 20);
    }
}

u32 mailbox::read(channel this_channel)
{
    read_write_options result;
    do
    {
        while(static_cast<bool>(mail_box_interface[0]->status & status_options::empty))
            ;
        result = mail_box_interface[0]->read_write;
    } while(static_cast<channel>(result & read_write_options::channel_mask) != this_channel);
    return static_cast<u32>(result & ~read_write_options::channel_mask) - 0x40000000;
}

void mailbox::write(u32 data, channel this_channel)
{
    while(static_cast<bool>(mail_box_interface[1]->status & status_options::full))
        ;
    mail_box_interface[1]->read_write = (static_cast<read_write_options>(0x40000000 + data) & ~read_write_options::channel_mask) | static_cast<read_write_options>(this_channel);
}
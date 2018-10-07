#include "mailbox.h"
#include "delay_loop.h"
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
    while(!(static_cast<bool>(mail_box_0_interface->status & status_options::empty)))
    {
        mail_box_0_interface->read_write;
        delay_loop(350000 * 20);
    }
}

u32 mailbox::read(channel a_channel)
{
    read_write_options result;
    do
    {
        while(static_cast<bool>(mail_box_0_interface->status & status_options::empty))
            ;
        result = mail_box_0_interface->read_write;
    } while(static_cast<channel>(result & read_write_options::channel_mask) != a_channel);
    return static_cast<u32>(result & ~read_write_options::channel_mask);
}

void mailbox::write(u32 data, channel a_channel)
{
    while(static_cast<bool>(mail_box_1_interface->status & status_options::full))
        ;
    mail_box_1_interface->read_write = (static_cast<read_write_options>(0xc0000000 + data) &
                                        ~read_write_options::channel_mask) |
                                       static_cast<read_write_options>(a_channel);
}
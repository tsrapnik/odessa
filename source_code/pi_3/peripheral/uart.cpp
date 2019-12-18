#include "uart.h"
#include "assert.h"

#include "string_.h" //todo: remove.

bool uart::device_used[device_count] = {false};
constexpr uart::registers* uart::registers_base_address[device_count];

uart::uart(device device_id, gpio* tx_pin, gpio* rx_pin) :
    tx_pin(tx_pin),
    rx_pin(rx_pin),
    char_buffer(100)
{
    //mark the device as used to avoid another instance of this device can be made.
    device_used[static_cast<u32>(device_id)] = true;

    //set the device_id, so it can be checked at runtime which device this is.
    this->device_id = device_id;

    the_registers = registers_base_address[static_cast<u32>(device_id)];

    //create an interrupt and pass this object as the source. if an interrupt should happen now the handler of this object
    //will get called at the appropriate time.
    the_interrupt = interrupt::create(interrupt::device::uart_interrupt, this);
    assert(the_interrupt != nullptr); //should never fail, if so we have a bug somewhere.

    //set baud rate.
    u32 clock_rate = 48000000;
    u32 baud_rate_times_16 = 115200 * 16;
    u32 fraction = (clock_rate % baud_rate_times_16) * 8 / 115200;

    registers::ibrd_struct temp_ibrd = {};
    temp_ibrd.ibrd = clock_rate / baud_rate_times_16;
    the_registers->ibrd = temp_ibrd;

    registers::fbrd_struct temp_fbrd = {};
    temp_fbrd.fbrd = fraction / 2 + fraction % 2;
    the_registers->fbrd = temp_fbrd;

    //set character length and enable fifo buffer.
    registers::lcrh_struct temp_lcrh = {};
    temp_lcrh.wlen = registers::lcrh_struct::wlen_enum::bits_8;
    temp_lcrh.fen = bool32::true_;
    the_registers->lcrh = temp_lcrh;

    //enable uart and transmitting.
    registers::cr_struct temp_cr = {};
    temp_cr.uarten = bool32::true_;
    temp_cr.txe = bool32::true_;
    the_registers->cr = temp_cr;

    //set when transmit interrupt should occur.
    registers::ifls_struct temp_ifls = {};
    temp_ifls.txiflsel = registers::ifls_struct::txiflsel_enum::fifo_1_8_full;
    the_registers->ifls = temp_ifls;

    //enable transmit interrupt.
    registers::imsc_struct temp_imsc = {};
    temp_imsc.txim = bool32::true_;
    the_registers->imsc = temp_imsc;

    //clear transmit interrupt set.
    registers::icr_struct temp_icr = {};
    temp_icr.txic = bool32::true_;
    the_registers->icr = temp_icr;
}

uart::~uart()
{
    //unmark the device as used, so it can be created again when needed.
    device_used[static_cast<u32>(device_id)] = false;

    //todo: make sure device gets destroyed and deinitialized.
    delete tx_pin;
    delete rx_pin;

    delete the_interrupt;
}

uart* uart::create(device device_id)
{
    //check if the device isn't already used, if so return a nullptr.
    //else create the device and return its pointer.
    if(device_used[static_cast<u32>(device_id)])
        return nullptr;
    else
    {
        //try to create the necessary gpio's first.
        gpio* tx_pin = gpio::create(gpio::device::gpio_14,
                                    gpio::pull_up_down_state::disable_pull_up_or_down,
                                    gpio::function::alternate_function_0);
        if(tx_pin == nullptr)
        {
            return nullptr;
        }
        gpio* rx_pin = gpio::create(gpio::device::gpio_15,
                                    gpio::pull_up_down_state::disable_pull_up_or_down,
                                    gpio::function::alternate_function_0);
        if(rx_pin == nullptr)
        {
            delete tx_pin;
            return nullptr;
        }

        //if all gpio's were created create the device.
        uart* new_device = new uart(device_id, tx_pin, rx_pin);
        return new_device;
    }
}

//write characters nonblocking to a buffer. the uart will afterwards transmit all characters
//in the buffer. if the buffer is full characters will get dropped.
void uart::write(const char* a_string)
{
    u32 index = 0;
    //push as many characters as possible directly in the fifo.
    for(; (a_string[index] != '\0') && (the_registers->fr.txff == bool32::false_); index++)
    {
        the_registers->dr.data = a_string[index];
    }

    //push other characters in a buffer. when the fifo has room it will automatically trigger an
    //interrupt to push new characters from the buffer in the fifo.
    for(; a_string[index] != '\0'; index++)
    {
        char_buffer.push(a_string[index]);
    }
}

bool uart::interrupt_occured()
{
    return the_registers->mis.txmis == bool32::true_;
}

void uart::handle_interrupt()
{
    //write characters to the transmit fifo until it is full or there are no more characters.
    u32 queue_length = char_buffer.get_queue_length();
    for(u32 index = 0; (index < queue_length) && (the_registers->fr.txff == bool32::false_); index++)
    {
        the_registers->dr.data = char_buffer.pop();
    }

    //clear interrupt flag.
    registers::icr_struct temp_icr = {};
    temp_icr.txic = bool32::true_;
    the_registers->icr = temp_icr;
}
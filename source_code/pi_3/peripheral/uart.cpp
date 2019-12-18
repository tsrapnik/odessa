#include "uart.h"

bool uart::device_used[device_count] = {false};
constexpr uart::registers* uart::registers_base_address[device_count];

uart::uart(device device_id, gpio* gpio_32, gpio* gpio_33, gpio* tx_pin, gpio* rx_pin) :
    gpio_32(gpio_32),
    gpio_33(gpio_33),
    tx_pin(tx_pin),
    rx_pin(rx_pin)
{
    //mark the device as used to avoid another instance of this device can be made.
    device_used[static_cast<u32>(device_id)] = true;

    //set the device_id, so it can be checked at runtime which device this is.
    this->device_id = device_id;

    the_registers = registers_base_address[static_cast<u32>(device_id)];

    u32 clock_rate = 48000000;
    u32 baud_rate_times_16 = 115200 * 16;
    u32 fraction = (clock_rate % baud_rate_times_16) * 8 / 115200;

    registers::imsc_struct temp_imsc = {};
    the_registers->imsc = temp_imsc;

    registers::icr_struct temp_icr = {};
    temp_icr.ctsmic = bool32::true_;
    temp_icr.rxic = bool32::true_;
    temp_icr.txic = bool32::true_;
    temp_icr.rtic = bool32::true_;
    temp_icr.feic = bool32::true_;
    temp_icr.peic = bool32::true_;
    temp_icr.beic = bool32::true_;
    temp_icr.oeic = bool32::true_;
    the_registers->icr = temp_icr;

    registers::ibrd_struct temp_ibrd = {};
    temp_ibrd.ibrd = clock_rate / baud_rate_times_16;
    the_registers->ibrd = temp_ibrd;

    registers::fbrd_struct temp_fbrd = {};
    temp_fbrd.fbrd = fraction / 2 + fraction % 2;
    the_registers->fbrd = temp_fbrd;

    registers::lcrh_struct temp_lcrh = {};
    temp_lcrh.wlen = registers::lcrh_struct::wlen_enum::bits_8;
    the_registers->lcrh = temp_lcrh;

    registers::ifls_struct temp_ifls = {};
    temp_ifls.txiflsel = registers::ifls_struct::txiflsel_enum::fifo_1_8_full;
    the_registers->ifls = temp_ifls;

    registers::cr_struct temp_cr = {};
    temp_cr.uarten = bool32::true_;
    temp_cr.txe = bool32::true_;
    the_registers->cr = temp_cr;
}

uart::~uart()
{
    //unmark the device as used, so it can be created again when needed.
    device_used[static_cast<u32>(device_id)] = false;

    //todo: make sure device gets destroyed and deinitialized.
    delete gpio_32;
    delete gpio_33;
    delete tx_pin;
    delete rx_pin;
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
        gpio* gpio_32 = gpio::create(gpio::device::gpio_32,
                                     gpio::pull_up_down_state::disable_pull_up_or_down,
                                     gpio::function::input);
        if(gpio_32 == nullptr)
        {
            return nullptr;
        }
        gpio* gpio_33 = gpio::create(gpio::device::gpio_33,
                                     gpio::pull_up_down_state::disable_pull_up_or_down,
                                     gpio::function::input);
        if(gpio_33 == nullptr)
        {
            delete gpio_32;
            return nullptr;
        }
        gpio* tx_pin = gpio::create(gpio::device::gpio_14,
                                    gpio::pull_up_down_state::disable_pull_up_or_down,
                                    gpio::function::alternate_function_0);
        if(tx_pin == nullptr)
        {
            delete gpio_32;
            delete gpio_33;
            return nullptr;
        }
        gpio* rx_pin = gpio::create(gpio::device::gpio_15,
                                    gpio::pull_up_down_state::disable_pull_up_or_down,
                                    gpio::function::alternate_function_0);
        if(rx_pin == nullptr)
        {
            delete gpio_32;
            delete gpio_33;
            delete tx_pin;
            return nullptr;
        }

        //if all gpio's were created create the device.
        uart* new_device = new uart(device_id, gpio_32, gpio_33, tx_pin, rx_pin);
        return new_device;
    }
}

void uart::write(const char* string)
{
    for(u32 index = 0; string[index] != '\0'; index++)
    {
        while(the_registers->fr.txff == bool32::true_)
            ;
        the_registers->dr.data = string[index];
    }
}

//todo: make real string input.
void uart::write(char* string, u32 size)
{
    for(u32 index = 0; index < size; index++)
    {
        while(the_registers->fr.txff == bool32::true_)
            ;
        the_registers->dr.data = string[index];
    }
}
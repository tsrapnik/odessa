#include "uart.h"
#include "enum_flags.h"

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

    this_registers = registers_base_address[static_cast<u32>(device_id)];

    u32 clock_rate = 48000000;
    u32 baud_rate_times_16 = 115200 * 16;
    u32 fraction = (clock_rate % baud_rate_times_16) * 8 / 115200;

    this_registers->imsc = 0;
    this_registers->icr = 0x7ff;
    this_registers->ibrd = clock_rate / baud_rate_times_16;
    this_registers->fbrd = fraction / 2 + fraction % 2;
    this_registers->lcrh = registers::lcrh_options::wlen8;
    this_registers->ifls = 0;
    this_registers->cr = registers::cr_options::uart_enable |
                         registers::cr_options::txe |
                         registers::cr_options::rxe;
}

uart::~uart()
{
    //unmark the device as used, so it can be created again when needed.
    device_used[static_cast<u32>(device_id)] = false;

    //todo: make sure device gets destroyed.
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
        //todo: hangs here in o0.
        while(static_cast<bool>(this_registers->fr & registers::fr::txff))
            ;
        this_registers->dr = string[index];
    }
}

//todo: make real string input.
void uart::write(char* string, u32 size)
{
    for(u32 index = 0; index < size; index++)
    {
        while(static_cast<bool>(this_registers->fr & registers::fr::txff))
        {
        }
        this_registers->dr = string[index];
    }
}

char* uart::read(u32& size)
{
    u32 index = 0;
    char* a_string = new char[size];
    while(!static_cast<bool>(this_registers->fr & registers::fr::rxfe) and index < size)
    {
        a_string[index] = this_registers->dr;
        size++;
    }
    size = index;
    return a_string;
}
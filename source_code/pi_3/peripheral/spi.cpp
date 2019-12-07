#include "spi.h"
#include "enum_flags.h"
#include "interrupt.h"

bool spi::device_used[device_count] = {false};
constexpr spi::registers* spi::registers_base_address[device_count];

spi::spi(device device_id, gpio* mosi, gpio* sclk, gpio* cs_pins[]) :
    mosi(mosi),
    sclk(sclk)
{
    for(usize index = 0; index < cs_count; index++)
        this->cs_pins[index] = cs_pins[index];

    //mark the device as used to avoid another instance of this device can be made.
    device_used[static_cast<u32>(device_id)] = true;

    //set the device_id, so it can be checked at runtime which device this is.
    this->device_id = device_id;

    the_registers = registers_base_address[static_cast<u32>(device_id)];

    the_registers->clk.cdiv = 0;

    registers::cs_struct tmp_cs = {};
    tmp_cs.cpha = registers::cs_struct::cpha_enum::first_sclk_at_middle_of_data;
    tmp_cs.cpol = registers::cs_struct::cpol_enum::rest_is_low;
    tmp_cs.clear = registers::cs_struct::clear_enum::cleart_tx_and_rx_fifo;
    tmp_cs.cspol = registers::cs_struct::cspol_enum::cs_active_low;
    tmp_cs.dmaen = false;
    tmp_cs.intd = false;;
    tmp_cs.intr = false;
    tmp_cs.ren = false;
    tmp_cs.len = registers::cs_struct::len_enum::spi_master;
    tmp_cs.cspol0 = registers::cs_struct::cspol_enum::cs_active_low;
    tmp_cs.cspol1 = registers::cs_struct::cspol_enum::cs_active_low;
    tmp_cs.cspol2 = registers::cs_struct::cspol_enum::cs_active_low;
    
    the_registers->cs = tmp_cs;
}

spi::~spi()
{
    //unmark the device as used, so it can be created again when needed.
    device_used[static_cast<u32>(device_id)] = false;

    //todo: release all resources.
}

spi* spi::create(device device_id)
{
    //check if the device isn't already used, if so return a nullptr.
    //else create the device and return its pointer.
    if(device_used[static_cast<u32>(device_id)])
        return nullptr;
    else
    {
        //todo: choose pins based on some configuration array, so it works also when there are multiple spi devices.
        //try to create the necessary gpio's first.
        gpio* mosi = gpio::create(gpio::device::gpio_10,
                                  gpio::pull_up_down_state::disable_pull_up_or_down,
                                  gpio::function::alternate_function_0);
        if(mosi == nullptr)
        {
            return nullptr;
        }
        gpio* sclk = gpio::create(gpio::device::gpio_11,
                                  gpio::pull_up_down_state::disable_pull_up_or_down,
                                  gpio::function::alternate_function_0);
        if(sclk == nullptr)
        {
            delete mosi;
            return nullptr;
        }
        gpio* cs_pins[cs_count];
        cs_pins[static_cast<usize>(cs_enum::cs0)] = gpio::create(gpio::device::gpio_8,
                                                       gpio::pull_up_down_state::disable_pull_up_or_down,
                                                       gpio::function::alternate_function_0);
        if(cs_pins[static_cast<usize>(cs_enum::cs0)] == nullptr)
        {
            delete mosi;
            delete sclk;
            return nullptr;
        }
        cs_pins[static_cast<usize>(cs_enum::cs1)] = gpio::create(gpio::device::gpio_7,
                                                       gpio::pull_up_down_state::disable_pull_up_or_down,
                                                       gpio::function::alternate_function_0);
        if(cs_pins[static_cast<usize>(cs_enum::cs1)] == nullptr)
        {
            delete mosi;
            delete sclk;
            delete cs_pins[static_cast<usize>(cs_enum::cs0)];
            return nullptr;
        }

        //if all gpio's were created create the device.
        spi* new_device = new spi(device_id, mosi, sclk, cs_pins);

        return new_device;
    }
}

void spi::write(cs_enum chip, w8* words, u32 word_count)
{
    //select correct chip.
    the_registers->cs.cs = chip;

    //clear fifo's.
    the_registers->cs.clear = registers::cs_struct::clear_enum::cleart_tx_and_rx_fifo;

    //enable transmission.
    the_registers->cs.ta = true;

    u32 transmit_count = 0;
    u32 receive_count = 0;
    while (true)
    {
        registers::cs_struct temp_cs = {};
        temp_cs = the_registers->cs; //todo: always necessary to read as a whole word, otherwise byte accessses.
        if(temp_cs.txd && (transmit_count < word_count))
        {
            the_registers->fifo = static_cast<u32>(words[transmit_count]);
            transmit_count++;
        }
        temp_cs = the_registers->cs;
        if(temp_cs.rxd && (receive_count < word_count))
        {
            u32 received = the_registers->fifo;
            //we are not doing anything with the received word, so just cast it to void,
            //to avoid compiler warnings.
            (void)received;
            receive_count++;
        }
        temp_cs = the_registers->cs;
        if((transmit_count == word_count) && (receive_count == word_count) && temp_cs.done)
            break;
    }

    //disable transmission.
    the_registers->cs.ta = false;
}
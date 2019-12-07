#include "i2s.h"
#include "enum_flags.h"
#include "interrupt.h"

bool i2s::device_used[device_count] = {false};
constexpr i2s::registers* i2s::registers_base_address[device_count];

i2s::i2s(device device_id, gpio* pcm_clk, gpio* pcm_fs, gpio* pcm_din, gpio* pcm_dout) :
    pcm_clk(pcm_clk),
    pcm_fs(pcm_fs),
    pcm_din(pcm_din),
    pcm_dout(pcm_dout)
{
    //mark the device as used to avoid another instance of this device can be made.
    device_used[static_cast<u32>(device_id)] = true;

    //set the device_id, so it can be checked at runtime which device this is.
    this->device_id = device_id;

    the_registers = registers_base_address[static_cast<u32>(device_id)];
    //todo: copy struct literals to registers.
    //enable pcm block.
    registers::cs_a_struct temp_cs_a = {};
    temp_cs_a.en = true;
    temp_cs_a.dmaen = false;
    temp_cs_a.rxthr = registers::cs_a_struct::rxthr_enum::fifo_single_sample;
    temp_cs_a.txthr = registers::cs_a_struct::txthr_enum::fifo_full_but_one;
    temp_cs_a.rxsex = true;
    temp_cs_a.stby = registers::cs_a_struct::stdby_enum::disable;

    the_registers->cs_a = temp_cs_a;

    u32 channel_length = 64;

    //define frame and channel settings.
    registers::mode_a_struct temp_mode_a = {};
    temp_mode_a.clk_dis = false;
    temp_mode_a.pdme = false;
    temp_mode_a.frxp = registers::mode_a_struct::frxp_enum::single_channel;
    temp_mode_a.ftxp = registers::mode_a_struct::ftxp_enum::single_channel;
    temp_mode_a.clkm = registers::mode_a_struct::clkm_enum::slave;
    temp_mode_a.clki = true; //todo: i2s requires reading on positive flank and changing value on negative flank, is inverse of default.
    temp_mode_a.fsm = registers::mode_a_struct::fsm_enum::slave;
    temp_mode_a.fsi = true;
    // temp_mode_a.fslen = channel_length; //todo: not really necessary, but just for completeness.
    // temp_mode_a.flen = 2 * channel_length - 1;

    the_registers->mode_a = temp_mode_a;

    registers::xc_a_struct temp_configuration = {};
    temp_configuration.ch1wex = true;
    temp_configuration.ch1en = true;
    temp_configuration.ch1pos = 0 + 1;
    temp_configuration.ch1wid = 0;
    temp_configuration.ch2wex = true;
    temp_configuration.ch2en = true;
    temp_configuration.ch2pos = channel_length + 1;
    temp_configuration.ch2wid = 0;

    the_registers->rxc_a = temp_configuration;

    temp_configuration.ch1wex = true;
    temp_configuration.ch1en = true;
    temp_configuration.ch1pos = 0 + 1;
    temp_configuration.ch1wid = 0;
    temp_configuration.ch2wex = true;
    temp_configuration.ch2en = true;
    temp_configuration.ch2pos = channel_length + 1;
    temp_configuration.ch2wid = 0;

    the_registers->txc_a = temp_configuration;

    //clear fifo's.
    temp_cs_a = the_registers->cs_a;
    temp_cs_a.rxclr = true;
    temp_cs_a.txclr = true;
    temp_cs_a.sync = true;

    the_registers->cs_a = temp_cs_a;

    //todo: clock sync does not work.
    for(volatile usize index = 0; index < 100; index++)
        ;
    // while(!the_registers->cs_a.sync)
    //     ;

    // //todo: enable ram needed?
    // //disable ram standby.
    // the_registers->cs_a.stby = true;

    // for(volatile usize index = 0; index < 100; index++)
    // ;

    //enable interrupt.
    // //todo: enable error interrupt?
    // registers::inten_a_struct temp_inten_a;
    // temp_inten_a = the_registers->inten_a;
    // temp_inten_a.rxr = true;
    // temp_inten_a.txw = true;

    // the_registers->inten_a = temp_inten_a;

    //fill transmit fifo with zeroes.
    for(usize index = 0; index < 64; index++)
    {
        the_registers->fifo_a = 0;
    }

    //start transmitting and receiving.
    temp_cs_a = the_registers->cs_a;
    temp_cs_a.txon = true;
    temp_cs_a.rxon = true;

    the_registers->cs_a = temp_cs_a;

    // AZO234 implementation.

    // //enable interrupt.
    // //todo: enable error interrupt?
    // registers::inten_a_struct temp_inten_a = {};
    // temp_inten_a.rxr = true;
    // temp_inten_a.txw = true;

    // the_registers->inten_a = temp_inten_a;

    // //define frame and channel settings.
    // registers::mode_a_struct temp_mode_a = {};
    // temp_mode_a.clk_dis = false;
    // temp_mode_a.pdme = false;
    // temp_mode_a.frxp = registers::mode_a_struct::frxp_enum::single_channel;
    // temp_mode_a.ftxp = registers::mode_a_struct::ftxp_enum::single_channel;
    // temp_mode_a.clkm = registers::mode_a_struct::clkm_enum::slave;
    // temp_mode_a.clki = false;
    // temp_mode_a.fsm = registers::mode_a_struct::fsm_enum::slave;
    // temp_mode_a.fsi = false;
    // //todo: remove cause only used in master sync mode.
    // temp_mode_a.flen = 64 - 1;
    // temp_mode_a.fslen = 32;

    // the_registers->mode_a = temp_mode_a;

    // registers::xc_a_struct temp_configuration = {};
    // temp_configuration.ch1wex = true;
    // temp_configuration.ch1en = true;
    // temp_configuration.ch1pos = 0;
    // temp_configuration.ch1wid = 8;
    // temp_configuration.ch2wex = true;
    // temp_configuration.ch2en = true;
    // temp_configuration.ch2pos = 32;
    // temp_configuration.ch2wid = 8;

    // the_registers->rxc_a = temp_configuration;
    // the_registers->txc_a = temp_configuration;

    // //todo: needed?
    // temp_configuration.ch1wex = true;
    // temp_configuration.ch1en = true;
    // temp_configuration.ch1pos = 1;
    // temp_configuration.ch1wid = 8;
    // temp_configuration.ch2wex = true;
    // temp_configuration.ch2en = true;
    // temp_configuration.ch2pos = 33;
    // temp_configuration.ch2wid = 8;

    // the_registers->rxc_a = temp_configuration;
    // the_registers->txc_a = temp_configuration;

    // //enable pcm block.
    // registers::cs_a_struct temp_cs_a = {};
    // temp_cs_a.en = true;
    // temp_cs_a.dmaen = false;
    // temp_cs_a.rxthr = registers::cs_a_struct::rxthr_enum::fifo_single_sample;
    // temp_cs_a.txthr = registers::cs_a_struct::txthr_enum::fifo_full_but_one;
    // temp_cs_a.rxclr = true;
    // temp_cs_a.txclr = true;
    // temp_cs_a.sync = true;
    // temp_cs_a.txon = true;
    // temp_cs_a.rxon = true;
    // temp_cs_a.rxsync = true;
    // temp_cs_a.txsync = true;

    // the_registers->cs_a = temp_cs_a;

    // temp_cs_a.sync = false;

    // the_registers->cs_a = temp_cs_a;

    // //todo: clock sync does not work.
    // // for(volatile usize index = 0; index < 100; index++)
    // //     ;
    // while(the_registers->cs_a.sync)
    //     ;
}

i2s::~i2s()
{
    //unmark the device as used, so it can be created again when needed.
    device_used[static_cast<u32>(device_id)] = false;

    //todo: release all resources.
}

i2s* i2s::create(device device_id)
{
    //check if the device isn't already used, if so return a nullptr.
    //else create the device and return its pointer.
    if(device_used[static_cast<u32>(device_id)])
        return nullptr;
    else
    {
        //todo: choose pins based on some configuration array, so it works also when there are multiple i2s devices.
        //try to create the necessary gpio's first.
        gpio* pcm_clk = gpio::create(gpio::device::gpio_18,
                                     gpio::pull_up_down_state::disable_pull_up_or_down,
                                     gpio::function::alternate_function_0);
        if(pcm_clk == nullptr)
        {
            return nullptr;
        }
        gpio* pcm_fs = gpio::create(gpio::device::gpio_19,
                                    gpio::pull_up_down_state::disable_pull_up_or_down,
                                    gpio::function::alternate_function_0);
        if(pcm_fs == nullptr)
        {
            delete pcm_clk;
            return nullptr;
        }
        gpio* pcm_din = gpio::create(gpio::device::gpio_20,
                                     gpio::pull_up_down_state::disable_pull_up_or_down,
                                     gpio::function::alternate_function_0);
        if(pcm_din == nullptr)
        {
            delete pcm_clk;
            delete pcm_fs;
            return nullptr;
        }
        gpio* pcm_dout = gpio::create(gpio::device::gpio_21,
                                      gpio::pull_up_down_state::disable_pull_up_or_down,
                                      gpio::function::alternate_function_0);
        if(pcm_dout == nullptr)
        {
            delete pcm_clk;
            delete pcm_fs;
            delete pcm_din;
            return nullptr;
        }

        //todo: solve interrupt needs to be created after device.
        //try to set the interrupt.
        // interruptable* a_interruptable = nullptr;
        // interrupt* i2s_interrupt = interrupt::create(interrupt::device::i2s_interrupt, a_interruptable);
        // (void)i2s_interrupt;

        //if all gpio's were created create the device.
        i2s* new_device = new i2s(device_id, pcm_clk, pcm_fs, pcm_din, pcm_dout);

        return new_device;
    }
}

bool i2s::transmit_required()
{
    registers::cs_a_struct tmp_cs_a;
    tmp_cs_a = the_registers->cs_a;
    return tmp_cs_a.txd; //todo: change to txd.
}

void i2s::transmit(i32 sample)
{
    the_registers->fifo_a = sample;
}

bool i2s::receive_required()
{
    registers::cs_a_struct tmp_cs_a;
    tmp_cs_a = the_registers->cs_a;
    return tmp_cs_a.rxd; //todo: change to rxd.
}

i32 i2s::receive()
{
    return the_registers->fifo_a;
}

bool i2s::transmit_error()
{
    registers::cs_a_struct tmp_cs_a;
    tmp_cs_a = the_registers->cs_a;
    return tmp_cs_a.txerr;
}

bool i2s::receive_error()
{
    registers::cs_a_struct tmp_cs_a;
    tmp_cs_a = the_registers->cs_a;
    return tmp_cs_a.rxerr;
}

void i2s::clear_transmit_error()
{
    //stop transmitting and receiving.
    registers::cs_a_struct temp_cs_a;
    temp_cs_a = the_registers->cs_a;
    temp_cs_a.txon = false;
    temp_cs_a.rxon = false;

    the_registers->cs_a = temp_cs_a;

    //clear fifo's.
    temp_cs_a = the_registers->cs_a;
    temp_cs_a.rxclr = true;
    temp_cs_a.txclr = true;
    temp_cs_a.sync = true;

    the_registers->cs_a = temp_cs_a;

    //todo: clock sync does not work.
    for(volatile usize index = 0; index < 100; index++)
        ;
    // while(!the_registers->cs_a.sync)
    //     ;

    //fill transmit fifo with zeroes.
    for(usize index = 0; index < 64; index++)
    {
        the_registers->fifo_a = 0;
    }

    //start transmitting and receiving.
    temp_cs_a = the_registers->cs_a;
    temp_cs_a.txon = true;
    temp_cs_a.rxon = true;

    the_registers->cs_a = temp_cs_a;
}

void i2s::clear_receive_error()
{
    clear_transmit_error();
}

i2s::channel i2s::pending_transmit_channel()
{
    registers::cs_a_struct tmp_cs_a;
    tmp_cs_a = the_registers->cs_a;
    return tmp_cs_a.txsync ? channel::left : channel::right;
}

i2s::channel i2s::pending_receive_channel()
{
    registers::cs_a_struct tmp_cs_a;
    tmp_cs_a = the_registers->cs_a;
    return tmp_cs_a.rxsync ? channel::left : channel::right;
}

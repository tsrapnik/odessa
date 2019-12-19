#include "i2s.h"
#include "assert.h"
#include "enum_flags.h"
#include "interrupt.h"
#include "string_.h"
#include "uart.h" //todo: remove.

extern uart* a_uart;

bool i2s::device_used[device_count] = {false};
constexpr i2s::registers* i2s::registers_base_address[device_count];

i2s::i2s(device device_id, gpio* pcm_clk, gpio* pcm_fs, gpio* pcm_din, gpio* pcm_dout) :
    pcm_clk(pcm_clk),
    pcm_fs(pcm_fs),
    pcm_din(pcm_din),
    pcm_dout(pcm_dout),
    buffers_incoming{buffer_fifo<i32>(64), buffer_fifo<i32>(64)},
    buffers_outgoing{buffer_fifo<i32>(64), buffer_fifo<i32>(64)}
{
    //mark the device as used to avoid another instance of this device can be made.
    device_used[static_cast<u32>(device_id)] = true;

    //set the device_id, so it can be checked at runtime which device this is.
    this->device_id = device_id;

    the_registers = registers_base_address[static_cast<u32>(device_id)];

    //create an interrupt and pass this object as the source. if an interrupt should happen now the handler of this object
    //will get called at the appropriate time.
    the_interrupt = interrupt::create(interrupt::device::i2s_interrupt, this);
    assert(the_interrupt != nullptr); //should never fail, if so we have a bug somewhere.

    //todo: copy struct literals to registers.
    //enable pcm block.
    registers::cs_a_struct temp_cs_a = {};
    temp_cs_a.en = bool32::true_;
    temp_cs_a.dmaen = bool32::false_;
    temp_cs_a.rxthr = registers::cs_a_struct::rxthr_enum::fifo_at_least;
    temp_cs_a.rxsex = bool32::true_;
    temp_cs_a.stby = registers::cs_a_struct::stdby_enum::disable;
    the_registers->cs_a = temp_cs_a;

    u32 channel_length = 64;

    //define frame and channel settings.
    registers::mode_a_struct temp_mode_a = {};
    temp_mode_a.clk_dis = bool32::false_;
    temp_mode_a.pdme = bool32::false_;
    temp_mode_a.frxp = registers::mode_a_struct::frxp_enum::single_channel;
    temp_mode_a.ftxp = registers::mode_a_struct::ftxp_enum::single_channel;
    temp_mode_a.clkm = registers::mode_a_struct::clkm_enum::slave;
    temp_mode_a.clki = bool32::true_; //todo: i2s requires reading on positive flank and changing value on negative flank, is inverse of default.
    temp_mode_a.fsm = registers::mode_a_struct::fsm_enum::slave;
    temp_mode_a.fsi = bool32::true_;
    // temp_mode_a.fslen = channel_length; //todo: not really necessary, but just for completeness.
    // temp_mode_a.flen = 2 * channel_length - 1;
    the_registers->mode_a = temp_mode_a;

    registers::xc_a_struct temp_configuration = {};
    temp_configuration.ch1wex = bool32::true_;
    temp_configuration.ch1en = bool32::true_;
    temp_configuration.ch1pos = 0 + 1;
    temp_configuration.ch1wid = 0;
    temp_configuration.ch2wex = bool32::true_;
    temp_configuration.ch2en = bool32::true_;
    temp_configuration.ch2pos = channel_length + 1;
    temp_configuration.ch2wid = 0;
    the_registers->rxc_a = temp_configuration;

    temp_configuration = {};
    temp_configuration.ch1wex = bool32::true_;
    temp_configuration.ch1en = bool32::true_;
    temp_configuration.ch1pos = 0 + 1;
    temp_configuration.ch1wid = 0;
    temp_configuration.ch2wex = bool32::true_;
    temp_configuration.ch2en = bool32::true_;
    temp_configuration.ch2pos = channel_length + 1;
    temp_configuration.ch2wid = 0;
    the_registers->txc_a = temp_configuration;

    //clear fifo's.
    temp_cs_a = the_registers->cs_a;
    temp_cs_a.rxclr = bool32::true_;
    temp_cs_a.txclr = bool32::true_;
    temp_cs_a.sync = bool32::true_;
    the_registers->cs_a = temp_cs_a;

    //todo: clock sync does not work.
    for(volatile usize index = 0; index < 100; index++)
        ;
    // while(!the_registers->cs_a.sync)
    //     ;

    //enable interrupt.
    registers::inten_a_struct temp_inten_a = {};
    temp_inten_a.rxr = bool32::true_;
    the_registers->inten_a = temp_inten_a;

    //fill transmit fifo with zeroes.
    for(usize index = 0; index < 64; index++)
    {
        the_registers->fifo_a = 0;
    }

    //start transmitting and receiving.
    temp_cs_a = the_registers->cs_a;
    temp_cs_a.txon = bool32::true_;
    temp_cs_a.rxon = bool32::true_;
    the_registers->cs_a = temp_cs_a;
}

i2s::~i2s()
{
    //unmark the device as used, so it can be created again when needed.
    device_used[static_cast<u32>(device_id)] = false;

    //todo: release all resources and deinit registers.
    delete the_interrupt;
    delete pcm_clk;
    delete pcm_fs;
    delete pcm_din;
    delete pcm_dout;
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

        //if all gpio's were created create the device.
        i2s* new_device = new i2s(device_id, pcm_clk, pcm_fs, pcm_din, pcm_dout);

        return new_device;
    }
}

void i2s::push(i32 sample, channel the_channel)
{
    //disable interrupts in this function, because we are accessing the same resources as in
    //the interrupt handler.
    interrupt::disabler current_scope;

    buffers_outgoing[static_cast<u32>(the_channel)].push(sample);
}

u32 i2s::get_sample_count(channel the_channel)
{
    //disable interrupts in this function, because we are accessing the same resources as in
    //the interrupt handler.
    interrupt::disabler current_scope;

    return buffers_incoming[static_cast<u32>(the_channel)].get_queue_length();
}

i32 i2s::pop(channel the_channel)
{
    //disable interrupts in this function, because we are accessing the same resources as in
    //the interrupt handler.
    interrupt::disabler current_scope;

    return buffers_incoming[static_cast<u32>(the_channel)].pop();
}

bool i2s::interrupt_occured()
{
    return the_registers->intstc_a.rxr == registers::intstc_a_struct::status_and_clear::interrupt_occured;
}

void i2s::handle_interrupt()
{
    //receive all samples until the incoming fifo is empty.
    while(the_registers->cs_a.rxd == bool32::true_)
    {
        buffers_incoming[static_cast<u32>(next_incoming_channel)].push(the_registers->fifo_a);
        next_incoming_channel = (next_incoming_channel == channel::left) ? channel::right : channel::left;
    }

    //transmit all samples until the outgoing fifo is full or the buffer is empty.
    while((the_registers->cs_a.txd == bool32::true_) && (buffers_outgoing[static_cast<u32>(next_outgoing_channel)].get_queue_length() > 0))
    {
        the_registers->fifo_a = buffers_outgoing[static_cast<u32>(next_outgoing_channel)].pop();
        next_outgoing_channel = (next_outgoing_channel == channel::left) ? channel::right : channel::left;
    }

    //clear interrupt flags (by writing a 1 to all flags that are set).
    registers::intstc_a_struct tmp_intstc_a;
    tmp_intstc_a = the_registers->intstc_a;
    the_registers->intstc_a = tmp_intstc_a;
}
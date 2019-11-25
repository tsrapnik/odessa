#include "i2s.h"
#include "enum_flags.h"

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

    this_registers = registers_base_address[static_cast<u32>(device_id)];

    //enable pcm block.
    registers::control_and_status_struct temp_control_and_status;
    
    temp_control_and_status = this_registers->control_and_status;
    temp_control_and_status.enable_pcm = true;
    temp_control_and_status.enable_dma_request = false;
    temp_control_and_status.rx_threshold = registers::control_and_status_struct::rx_threshold_enum::fifo_single_sample;
    temp_control_and_status.tx_threshold = registers::control_and_status_struct::tx_threshold_enum::fifo_full_but_one;

    //define frame and channel settings.
    registers::mode_struct temp_mode;

    temp_mode = this_registers->mode;
    temp_mode.pcm_clock_disable = false;
    temp_mode.pdm_input_mode_enable = false;
    temp_mode.receive_frame_packed_mode = registers::mode_struct::receive_frame_packed_mode_enum::single_channel;
    temp_mode.transmit_frame_packed_mode = registers::mode_struct::transmit_frame_packed_mode_enum::single_channel;
    temp_mode.clock_mode = registers::mode_struct::clock_mode_enum::slave;
    temp_mode.clock_invert = false;
    temp_mode.frame_sync_mode = registers::mode_struct::frame_sync_mode_enum::slave;
    temp_mode.frame_sync_invert = false;

    this_registers->mode = temp_mode;

    registers::configuration_struct temp_configuration;

    temp_configuration.channel_1_width_extension = true;
    temp_configuration.channel_1_enable = true;
    temp_configuration.channel_1_position = 0;
    temp_configuration.channel_1_width = 8;
    temp_configuration.channel_2_width_extension = true;
    temp_configuration.channel_2_enable = true;
    temp_configuration.channel_2_position = 0;
    temp_configuration.channel_2_width = 8;

    this_registers->receive_configuration = temp_configuration;
    this_registers->transmit_configuration = temp_configuration;

    //clear fifo's.
    temp_control_and_status = this_registers->control_and_status;
    temp_control_and_status.clear_rx_fifo = true;
    temp_control_and_status.clear_tx_fifo = true;
    temp_control_and_status.pcm_clock_sync_helper = true;

    this_registers->control_and_status = temp_control_and_status;

    //todo: clock sync does not work.
    for(volatile usize index = 0; index < 100; index++)
        ;
    // while(!this_registers->control_and_status.pcm_clock_sync_helper)
    //     ;

    //todo: enable ram needed?
    //disable ram standby.
    this_registers->control_and_status.ram_not_in_standby = true;    

    for(volatile usize index = 0; index < 100; index++)
    ;

    //enable interrupts.
    //todo: enable error interrupts?
    registers::interrupt_enables_struct temp_interrupt_enables;
    temp_interrupt_enables = this_registers->interrupt_enables;
    temp_interrupt_enables.rx_read_interrupt_enable = true;
    temp_interrupt_enables.tx_write_interrupt_enable = true;

    this_registers->interrupt_enables = temp_interrupt_enables;

    //fill transmit fifo with zeroes.
    for(usize index = 0; index < 64; index++)
    {
        this_registers->fifo_data = 0;
    }

    //start transmitting and receiving.
    temp_control_and_status = this_registers->control_and_status;
    temp_control_and_status.enable_transmission = true;
    temp_control_and_status.enable_reception = true;

    this_registers->control_and_status = temp_control_and_status;
}

i2s::~i2s()
{
    //unmark the device as used, so it can be created again when needed.
    device_used[static_cast<u32>(device_id)] = false;
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

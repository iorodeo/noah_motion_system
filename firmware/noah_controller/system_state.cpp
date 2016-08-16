#include "system_state.h"

// System timer callback funtion
// ----------------------------------------------------------------------------------------------
// SystemState public methods
// ------------------------------------------------------------------------------------------------

SystemState::SystemState() { }

void SystemState::initialize()
{ 
    Serial.begin(115200);
    setup_stepper();
    setup_analog_input();
    setup_trigger_output();
    setup_digital_output();
    setup_pwm_output();
    setup_timer(); // Last
}

void SystemState::loop_update()
{
    for (int i=0; i<constants::NumStepper; i++)
    {
        Serial.print(stepper_[i].position()); 
        Serial.print(" ");
    }
    Serial.println();
    delay(5);

    //send_and_recv();
}


// SystemState private methods
// ------------------------------------------------------------------------------------------------

void SystemState::send_and_recv()
{
    if (new_msg_flag_)
    {
        new_msg_flag_ = false;
        if (!send_msg_to_host())
        {
            on_send_msg_error();
        }
        else
        {
            if (!recv_msg_from_host())
            {
                on_recv_msg_error();
            }
        }
    } 
}

bool SystemState::send_msg_to_host()
{ 
    bool rtn_val = true;
    DevToHostMsg dev_to_host_msg = create_dev_to_host_msg();
    int num_bytes = RawHID.send(&dev_to_host_msg,constants::DevToHostTimeout);
    if (num_bytes != sizeof(DevToHostMsg))
    {
        rtn_val = false;
    }
    return rtn_val;
}


bool SystemState::recv_msg_from_host()
{
    bool rtn_val = true;
    HostToDevMsg host_to_dev_msg;
    int num_bytes = RawHID.recv(&host_to_dev_msg,constants::HostToDevTimeout);
    if (num_bytes != sizeof(HostToDevMsg))
    {
        rtn_val = false;
    }
    else
    {
        command_switchyard(host_to_dev_msg);
    }
    return rtn_val;
}


DevToHostMsg SystemState::create_dev_to_host_msg()
{
    DevToHostMsg dev_to_host_msg;

    // Set status information
    dev_to_host_msg.status = 0;
    dev_to_host_msg.status = 0xf & mode_;  
    dev_to_host_msg.status |= (send_msg_error_flag_ <<  constants::NumModeBits);
    dev_to_host_msg.status |= (recv_msg_error_flag_ << (constants::NumModeBits+1));

    // Reset msg error flags
    send_msg_error_flag_ = false;
    recv_msg_error_flag_ = false;

    // Get current time in us
    uint32_t micros_curr = micros();
    uint32_t micros_dt = micros_curr - micros_last_;
    micros_last_ = micros_curr;
    time_us_ += uint64_t(micros_dt);
    dev_to_host_msg.time_us = time_us_;

    // Echo back message count for lag checking 
    dev_to_host_msg.count = uint8_t(msg_count_);

    // Read Analog inputs 
    for (int i=0; i<constants::NumAnalogInput; i++)
    {
        dev_to_host_msg.analog_input[i] = analogRead(constants::AnalogInputPinArray[i]);
    }
    return dev_to_host_msg;
}


void SystemState::command_switchyard(HostToDevMsg host_to_dev_msg)
{
    msg_count_ = host_to_dev_msg.count;
    command_ = constants::UsbCommand(host_to_dev_msg.command);

    switch (command_)
    {
        case constants::Cmd_Empty: 
            // Take no action 
            break;

        case constants::Cmd_SetMode_Disabled:
            set_mode_disabled();
            break;

        case constants::Cmd_SetMode_Enabled:
            set_mode_enabled();
            break;

        case constants::Cmd_SetMode_MoveToPosition:
            set_mode_move_to_position(host_to_dev_msg);
            break;

        case constants::Cmd_SetMode_HomeAxis:
            set_mode_home_axis(host_to_dev_msg);
            break;

        case constants::Cmd_SetMode_VelocityControl:
            break;

        case constants::Cmd_Get_TriggerCount:
            break;

        case constants::Cmd_Set_TriggerCount:
            break;

        case constants::Cmd_Get_TriggerEnabled:
            break;

        case constants::Cmd_Get_DigitalOutput:
            break;

        default:
            break;

    }
}


void SystemState::set_mode_disabled()
{
    digitalWrite(constants::StepperDriveEnablePin,LOW);
    enabled_flag_= false;
    mode_ = constants::Mode_Disabled;
}


void SystemState::set_mode_enabled()
{
    digitalWrite(constants::StepperDriveEnablePin,HIGH);
    enabled_flag_ = true;
    mode_ = constants::Mode_Enabled;
}


void SystemState::set_mode_move_to_position(HostToDevMsg host_to_dev_msg)
{

}


void SystemState::set_mode_home_axis(HostToDevMsg host_to_dev_msg)
{
}


void SystemState::on_send_msg_error()
{
    // Take correct actions of message error/timeout ... need to flush this out.
    //time_us_ = 0;
    send_msg_error_flag_ = true;
}

void SystemState::on_recv_msg_error()
{
    // Take correct actions of message error/timeout ... need to flush this out.
    //time_us_ = 0;
    recv_msg_error_flag_ = true;
}


// Private initialization methods
// ------------------------------------------------------------------------------------------------

void SystemState::setup_stepper()
{
    pinMode(constants::StepperDriveEnablePin, OUTPUT);
    digitalWrite(constants::StepperDriveEnablePin,LOW);
    enabled_flag_ = false;

    for (int i =0; i<constants::NumStepper; i++)
    {
        StepperPin pin = constants::StepperPinArray[i];
        stepper_[i] = Stepper(pin.clk,pin.dir); 
        stepper_[i].initialize();
        stepper_[i].set_velocity(-100);
        stepper_[i].set_min_position(constants::StepperMinimumPosition[i]);
        stepper_[i].set_max_position(constants::StepperMaximumPosition[i]);
        stepper_[i].disable_bounds_check();
    }
}


void SystemState::setup_analog_input()
{
    analogReadResolution(constants::AnalogReadNumBits);
    analogReadAveraging(constants::AnalogReadNumAvg);
    analogReference(constants::AnalogRefType);
}

void SystemState::setup_trigger_output()
{
    for (int i=0; i<constants::NumTrigger; i++)
    {
        trigger_[i] = Trigger(
                constants::TriggerPinArray[i],
                constants::DefaultTriggerCount[i],
                constants::DefaultTriggerEnabled[i]
                );
        trigger_[i].initialize();
    }
}

void SystemState::setup_digital_output()
{
    for (int i=0; i<constants::NumDigitalOutput; i++)
    {
        uint8_t pin = constants::DigitalOutputPinArray[i];
        pinMode(pin, OUTPUT);
        uint8_t value = constants::DefaultDigitalOutputValue[i];
        digitalWrite(pin,value);
    }
}

void SystemState::setup_pwm_output()
{
    analogWriteResolution(constants::PwmResolution);
    for (int i=0; i<constants::NumPwm; i++)
    {
        pinMode(constants::PwmPinArray[i],OUTPUT);
        analogWriteFrequency(constants::PwmPinArray[i],constants::PwmFrequency);
        analogWrite(constants::PwmPinArray[i],constants::PwmDefaultValue);
    }
}

void SystemState::setup_timer()
{
    timer_.priority(constants::TimerPriority);
    timer_.begin(timer_callback,constants::TimerPeriod);
}


// SystemState Instance
// ------------------------------------------------------------------------------------------------
SystemState system_state = SystemState();

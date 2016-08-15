#include "system_state.h"

// System timer callback funtion
// ----------------------------------------------------------------------------------------------
// SystemState public methods
// ------------------------------------------------------------------------------------------------

SystemState::SystemState() { }

void SystemState::initialize()
{ 
    setup_stepper();
    setup_analog_input();
    setup_trigger_output();
    setup_digital_output();
    setup_pwm_output();
    setup_timer();
}


void SystemState::send_and_recv()
{
    bool ok;
    if (new_msg_flag_)
    {
        new_msg_flag_ = false;
        ok = send_msg_to_host();
        if (!ok)
        {
            on_msg_error();
        }
        else
        {
            ok = recv_msg_from_host();
            if (!ok)
            {
                on_msg_error();
            }
        }
    } 
}


// SystemState private methods
// ------------------------------------------------------------------------------------------------

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

DevToHostMsg SystemState::create_dev_to_host_msg()
{
    DevToHostMsg dev_to_host_msg;

    // Set status

    // Get current time in us
    uint32_t micros_curr = micros();
    uint32_t micros_dt = micros_curr - micros_last_;
    micros_last_ = micros_curr;
    time_us_ += uint64_t(micros_dt);
    dev_to_host_msg.time_us = time_us_;

    // Read Analog inputs 
    for (int i=0; i<constants::NumAnalogInput; i++)
    {
        dev_to_host_msg.analog_input[i] = analogRead(constants::AnalogInputPinArray[i]);
    }
    return dev_to_host_msg;
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
        // Extract information from message and take action
    }
    return rtn_val;
}

void SystemState::on_msg_error()
{
    // Take correct actions of message error/timeout ... need to flush this out.
    time_us_ = 0;
}


// Private initialization methods
// ------------------------------------------------------------------------------------------------

void SystemState::setup_stepper()
{
    for (int i =0; i<constants::NumStepper; i++)
    {
        StepperPin pin = constants::StepperPinArray[i];
        stepper_[i] = Stepper(pin.clk,pin.dir); 
        stepper_[i].initialize();
        stepper_[i].set_velocity(0.0);
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
        pinMode(constants::TriggerPinArray[i],OUTPUT);
        digitalWrite(constants::TriggerPinArray[i],LOW);
        trigger_enabled_[i] = constants::DefaultTriggerEnabled[i];
        trigger_count_[i] = constants::DefaultTriggerCount[i];
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

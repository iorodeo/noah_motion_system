#include "system_state.h"
#include "rawhid_msg_types.h"

// System timer callback funtion
// ----------------------------------------------------------------------------------------------
inline void timer_callback()
{
    static uint32_t cnt = 0;
    if (cnt%constants::NewMessageCount == 0)
    {
        system_state.new_msg_flag_ = true;
    }
    cnt++;
}

// SystemState public methods
// ------------------------------------------------------------------------------------------------

SystemState::SystemState() { }

void SystemState::initialize()
{ 
    setup_analog_input();
    setup_trigger_output();
    setup_digital_output();
    setup_pwm_output();
    setup_timer();
}


void SystemState::update()
{
    DevToHostMsg dev_to_host_msg;
    HostToDevMsg host_to_dev_msg;
    int num_bytes = 0;

    if (new_msg_flag_)
    {
        new_msg_flag_ = false;

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

        // Send message to host
        num_bytes = RawHID.send(&dev_to_host_msg,constants::DevToHostTimeout);
        if (num_bytes != sizeof(DevToHostMsg))
        {
            // Message send failed
            time_us_ = 0;
        }
        else
        {
            // Get response back from host
            num_bytes = RawHID.recv(&host_to_dev_msg,constants::HostToDevTimeout);
            if (num_bytes == sizeof(HostToDevMsg))
            {
                // Response is OK
            }
        }
    }
}


// SystemState private methods
// ------------------------------------------------------------------------------------------------

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
    }
}

void SystemState::setup_digital_output()
{
    for (int i=0; i<constants::NumDigitalOutput; i++)
    {
        pinMode(constants::DigitalOutputPinArray[i], OUTPUT);
        digitalWrite(constants::DigitalOutputPinArray[i], LOW);
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

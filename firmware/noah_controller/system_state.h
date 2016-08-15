#ifndef SYSTEM_STATE_H
#define SYSTEM_STATE_H
#include "Arduino.h"
#include "constants.h"
#include "rawhid_msg_types.h"
#include "stepper.h"

class SystemState
{
    public:
        volatile bool new_msg_flag_ = false; 

        SystemState();
        void initialize();           
        void send_and_recv();               

        inline void update_stepper();
        inline void update_trigger();
        inline void update_new_msg_flag();
        inline void update_timer_count();

    private:
        IntervalTimer timer_;                     
        volatile uint32_t timer_count_ = 0;

        constants::OperatingMode mode_ = constants::Disabled;           
        bool enabled_flag_ = false;
        bool msg_error_flag_ = false;

        Stepper stepper_[constants::NumStepper];  

        bool trigger_enabled_[constants::NumTrigger]; 
        uint16_t trigger_count_[constants::NumTrigger]; 

        // Time variables
        uint32_t micros_last_ = 0;   // Value from last call to micros
        uint64_t time_us_ = 0;       // Elapsed time in us

        // Messaging methods
        bool send_msg_to_host();     // Send (raw hid) message to host
        bool recv_msg_from_host();   // Receive (raw hid) message from host
        void on_msg_error();         // Response to messaging error 
        DevToHostMsg create_dev_to_host_msg(); // Create message to send to host

        // Initialization methods
        void setup_stepper();
        void setup_analog_input();
        void setup_trigger_output();
        void setup_digital_output();
        void setup_pwm_output();
        void setup_timer();

};


extern SystemState system_state;


inline void SystemState::update_stepper()
{
    for (int i=0; i<constants::NumStepper; i++)
    {
        stepper_[i].update_dir_and_set_clk_high();
    }
    delayMicroseconds(constants::StepperClockHighDelay);
    for (int i=0; i<constants::NumStepper; i++)
    {
        stepper_[i].set_clk_low();
    }
}


inline void SystemState::update_trigger() 
{ 
    for (int i=0; i<constants::NumTrigger; i++)
    {
        if (trigger_enabled_[i] && (timer_count_%trigger_count_[i] == 0))
        {
            digitalWriteFast(constants::TriggerPinArray[i],HIGH);
        }
    }
    delayMicroseconds(constants::TriggerHighDelay);
    for (int i=0; i<constants::NumTrigger; i++)
    {
        if (trigger_enabled_[i])
        {
            digitalWriteFast(constants::TriggerPinArray[i],LOW);
        }
    }
}


inline void SystemState::update_new_msg_flag()
{
    if (system_state.timer_count_%constants::NewMessageCount == 0)
    {
        system_state.new_msg_flag_ = true;
    }
}


inline void SystemState::update_timer_count()
{
    system_state.timer_count_++;
}


inline void timer_callback()
{
    system_state.update_stepper();
    system_state.update_trigger();
    system_state.update_new_msg_flag();
    system_state.update_timer_count();
}



#endif 

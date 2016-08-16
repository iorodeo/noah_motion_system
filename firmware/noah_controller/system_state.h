#ifndef SYSTEM_STATE_H
#define SYSTEM_STATE_H
#include "Arduino.h"
#include "constants.h"
#include "rawhid_msg_types.h"
#include "stepper.h"
#include "trigger.h"
#include "velocity_controller.h"

class SystemState
{
    public:
        volatile bool new_msg_flag_ = false; 

        SystemState();
        void initialize();           
        void loop_update();

        // Update on timer methods
        inline void update_stepper();
        inline void update_trigger();
        inline void update_new_msg_flag();
        inline void update_timer_count();

    protected:
        IntervalTimer timer_;                     
        volatile uint32_t timer_count_ = 0;

        bool enabled_flag_ = false;
        constants::OperatingMode mode_ = constants::Mode_Disabled;           

        uint8_t msg_count_ = 0;
        bool send_msg_error_flag_ = false;
        bool recv_msg_error_flag_ = false;
        constants::UsbCommand command_ = constants::Cmd_Empty;

        Stepper stepper_[constants::NumStepper];  
        Trigger trigger_[constants::NumTrigger];

        // Controllers
        VelocityController velocityController_[constants::NumStepper]; 

        // Time variables
        uint32_t micros_last_ = 0;   // Value from last call to micros
        uint64_t time_us_ = 0;       // Elapsed time in us

        // Messaging methods
        void send_and_recv();               
        bool send_msg_to_host();     // Send (raw hid) message to host
        bool recv_msg_from_host();   // Receive (raw hid) message from host

        void on_send_msg_error();    // Response to send messaging error 
        void on_recv_msg_error();    // Response to recv messaging error 

        DevToHostMsg create_dev_to_host_msg();                 // Create new message to send to host
        void command_switchyard(HostToDevMsg host_to_dev_msg); // Take action in response to new message

        // Actions in reponse to usb commands
        void set_mode_enabled();
        void set_mode_disabled();
        void set_mode_move_to_position(HostToDevMsg host_to_dev_msg);
        void set_mode_home_axis(HostToDevMsg host_to_dev_msg);
        void set_mode_velocity_control(HostToDevMsg host_to_dev_msg);

        // Setup/Initialization methods
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
        trigger_[i].set_pin_high_on_count(timer_count_);
    }
    delayMicroseconds(constants::TriggerHighDelay);
    for (int i=0; i<constants::NumTrigger; i++)
    {
        trigger_[i].set_pin_low_on_count(timer_count_);
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

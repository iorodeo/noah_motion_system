#ifndef SYSTEM_STATE_H
#define SYSTEM_STATE_H
#include "Arduino.h"
#include "dev_constants.h"
#include "rawhid_msg_types.h"
#include "stepper.h"
#include "trigger.h"
#include "velocity_controller.h"
#include "position_controller.h"
#include "homing_controller.h"
#include "estop_monitor.h"

class SystemState
{
    public:

        volatile bool new_msg_flag_ = false; 

        SystemState();

        void initialize();           
        void update_on_loop(); 

        inline void update_stepper_on_timer();
        inline void update_trigger_on_timer();
        inline void update_new_msg_flag_on_timer();
        inline void update_count_on_timer();

    protected:

        constants::OperatingMode mode_ = constants::Mode_Disabled;           

        IntervalTimer timer_;                     
        volatile uint32_t timer_count_ = 0;

        bool send_msg_error_flag_ = false;
        bool recv_msg_error_flag_ = false;
        bool error_flag_ = false;

        HostToDevMsg host_to_dev_msg_last_;
        uint16_t command_response_data_;

        bool stop_motion_flag_= false;

        Stepper stepper_[constants::NumStepper];  
        Trigger trigger_[constants::NumTrigger];

        uint8_t homing_axis_ = 0;
        bool homed_flag_[constants::NumStepper];

        // Controllers/Monitors
        VelocityController velocity_controller_[constants::NumStepper]; 
        PositionController position_controller_[constants::NumStepper];
        HomingController homing_controller_[constants::NumStepper];
        EStopMonitor estop_monitor_;

        // Time variables
        uint32_t micros_last_ = 0;   // Value from last call to micros
        uint64_t time_us_ = 0;       // Elapsed time in us

        // Messaging methods
        void send_and_recv();               
        bool send_msg_to_host();     // Send (raw hid) message to host
        bool recv_msg_from_host();   // Receive (raw hid) message from host

        void on_send_msg_error();    // Response to send messaging error 
        void on_recv_msg_error();    // Response to recv messaging error 

        DevToHostMsg create_dev_to_host_msg();  // Create new message to send to host
        void command_switchyard();              // Take action in response to new message

        // Loop update methods for operating modes
        void update_modes_on_loop();  
        void update_disabled_on_loop();
        void update_ready_on_loop();
        void update_homing_on_loop();
        void update_positioning_on_loop();
        void update_velocity_control_on_loop();
        void update_stop_motion_on_loop();

        // Actions in reponse to usb commands
        void set_mode_disabled();
        void set_mode_ready(bool ignore_mode=false);
        void set_mode_homing();
        void set_mode_positioning();
        void set_mode_velocity_control();
        void stop_motion_cmd();
        void set_axis_homed_cmd();
        void get_axis_homed_cmd();
        void set_trigger_count_cmd();
        void get_trigger_count_cmd();
        void set_trigger_enabled_cmd();
        void get_trigger_enabled_cmd();

        void set_stepper_position_cmd();
        void set_stepper_min_position_cmd();
        void get_stepper_min_position_cmd();
        void set_stepper_max_position_cmd();
        void get_stepper_max_position_cmd();
        void set_stepper_max_speed_cmd();
        void get_stepper_max_speed_cmd();
        void set_stepper_max_accel_cmd();
        void get_stepper_max_accel_cmd();

        bool all_axes_homed();

        // Setup/Initialization methods
        void setup_stepper();
        void setup_velocity_controller();
        void setup_position_controller();
        void setup_analog_input();
        void setup_estop_monitor();
        void setup_trigger_output();
        void setup_digital_output();
        void setup_pwm_output();
        void setup_homing();
        void setup_timer();
};

extern SystemState system_state; 


// Timer update functions
// --------------------------------------------------------------------------------------

inline void SystemState::update_stepper_on_timer()
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


inline void SystemState::update_trigger_on_timer() 
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


inline void SystemState::update_new_msg_flag_on_timer()
{
    if (system_state.timer_count_%constants::NewMessageCount == 0)
    {
        system_state.new_msg_flag_ = true;
    }
}


inline void SystemState::update_count_on_timer()
{
    system_state.timer_count_++;
}


inline void timer_callback()
{
    system_state.update_stepper_on_timer();
    system_state.update_trigger_on_timer();
    system_state.update_new_msg_flag_on_timer();
    system_state.update_count_on_timer();
}


#endif 


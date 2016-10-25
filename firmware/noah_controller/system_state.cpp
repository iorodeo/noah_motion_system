#include "system_state.h"

// SystemState public methods
// ------------------------------------------------------------------------------------------------

SystemState::SystemState() { }

void SystemState::initialize()
{ 
    load_eeprom_config(); // Always first
    
    setup_stepper();
    setup_velocity_controller();
    setup_position_controller();
    setup_analog_input();
    setup_estop_monitor();
    setup_trigger_output();
    setup_digital_output();
    setup_pwm_output();
    setup_homing();
    setup_timer(); // Always last in setup

    // DEBUG
    // ------------------------------------------------
    //Serial.begin(115200);
    // ------------------------------------------------
}


void SystemState::update_on_loop()
{
    if (estop_monitor_.is_stopped() && (mode_ != constants::Mode_Disabled))
    {
        set_mode_disabled();
    }
    
    send_and_recv();        
    update_modes_on_loop();  

    if (stop_motion_flag_)
    {
        update_stop_motion_on_loop();
    }
}


// SystemState private methods
// ------------------------------------------------------------------------------------------------

void SystemState::update_modes_on_loop()
{
    switch (mode_)
    {
        case constants::Mode_Disabled:
            update_disabled_on_loop();
            break;

        case constants::Mode_Ready:
            update_ready_on_loop();
            break;

        case constants::Mode_Homing:
            update_homing_on_loop();
            break;
            
        case constants::Mode_Positioning:
            update_positioning_on_loop();
            break;
            
        case constants::Mode_VelocityControl:
            update_velocity_control_on_loop();
            break;
            
        default:
            break;
    }
}


void SystemState::update_stop_motion_on_loop()
{
    bool all_stopped = true;
    for (int i=0; i<constants::NumStepper; i++)
    {
        int32_t velocity = stepper_[i].velocity();
        all_stopped &= (velocity == 0);
    }

    if (all_stopped)
    {
        set_mode_ready(true);
        stop_motion_flag_ = false;
        for (int i=0; i<constants::NumDigitalOutput; i++)
        {
            uint8_t pin = constants::DigitalOutputPinArray[i];
            uint8_t value = constants::DefaultDigitalOutputValue[i];
            digitalWrite(pin,value);
        }
    }
}


void SystemState::update_disabled_on_loop()
{
}


void SystemState::update_ready_on_loop()
{
}


void SystemState::update_homing_on_loop()
{ 
    // Update homing controller using current stepper position to get 
    // next stepper velocity. 
    int32_t position = stepper_[homing_axis_].position();
    int32_t velocity = homing_controller_[homing_axis_].update(position);
    stepper_[homing_axis_].set_velocity(velocity);

    // Check to see if home has been found
    bool home_found = HomingController::home_found();
    if (home_found)
    {
        stepper_[homing_axis_].set_velocity(0);

        int32_t home_position;
        if (homing_controller_[homing_axis_].direction() > 0)
        {
            home_position = stepper_[homing_axis_].max_position();
        }
        else
        {
            home_position = stepper_[homing_axis_].min_position();
        }
        stepper_[homing_axis_].set_position(home_position);
        stepper_[homing_axis_].enable_bounds_check();
        homed_flag_[homing_axis_] = true;
        set_mode_ready(true);
    }
}


void SystemState::update_positioning_on_loop()
{ 
    bool done = true;

    for (int i=0; i<constants::NumStepper; i++)
    { 
        // Get current stepper position and update postion controller to get next
        // velocity set point.
        int32_t position = stepper_[i].position();
        int32_t velocity_setp = position_controller_[i].update(position);

        // Set velocity controller set point and update velocity controller
        // to get next stepper velocity.
        if (!stop_motion_flag_)
        {
            velocity_controller_[i].set_velocity_setp(velocity_setp);
        }
        int32_t velocity = velocity_controller_[i].update(position);
        stepper_[i].set_velocity(velocity);

        // Check for done
        int32_t position_setp = position_controller_[i].position_setp();
        if ((position != position_setp) || (velocity != 0))
        {
            done = false;
        }
    }

    if (done)
    {
        set_mode_ready(true);
    }

}


void SystemState::update_velocity_control_on_loop()
{
    for (int i=0; i<constants::NumStepper; i++)
    {
        if (!stop_motion_flag_)
        {
            velocity_controller_[i].set_velocity_setp(host_to_dev_msg_last_.stepper_velocity[i]);
        }
        int32_t position = stepper_[i].position();
        int32_t velocity = velocity_controller_[i].update(position);
        stepper_[i].set_velocity(velocity);
    }
    for (int i=0; i<constants::NumDigitalOutput; i++)
    {
        uint8_t pin = constants::DigitalOutputPinArray[i];
        uint8_t value = host_to_dev_msg_last_.digital_output[i];
        if (value > 0)
        {
            digitalWriteFast(pin,HIGH);
        }
        else
        {
            digitalWriteFast(pin,LOW);
        }
    }
}


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
    HostToDevMsg host_to_dev_msg = {};
    int num_bytes = RawHID.recv(&host_to_dev_msg,constants::HostToDevTimeout);
    if (num_bytes != sizeof(HostToDevMsg))
    {
        rtn_val = false;
    }
    else
    {
        host_to_dev_msg_last_ = host_to_dev_msg;
        command_switchyard();
    }
    return rtn_val;
}


DevToHostMsg SystemState::create_dev_to_host_msg()
{
    DevToHostMsg dev_to_host_msg = {};

    // Set status information
    dev_to_host_msg.status = 0;
    dev_to_host_msg.status = constants::ModeBitsMask & mode_;  

    dev_to_host_msg.status |= (stop_motion_flag_ << 4);
    if (send_msg_error_flag_ || recv_msg_error_flag_)
    {
        dev_to_host_msg.status  |= (1 << 5);
    }
    if (error_flag_)
    {
        dev_to_host_msg.status |= (1 << 6);
    }

    // Reset error flags
    send_msg_error_flag_ = false;
    recv_msg_error_flag_ = false;
    error_flag_ = false;

    // Get current time in us
    uint32_t micros_curr = micros();
    uint32_t micros_dt = micros_curr - micros_last_;
    micros_last_ = micros_curr;
    time_us_ += uint64_t(micros_dt);
    dev_to_host_msg.time_us = time_us_;

    for (int i=0; i<constants::NumStepper; i++)
    {
        dev_to_host_msg.stepper_position[i] = stepper_[i].position();
        dev_to_host_msg.stepper_velocity[i] = stepper_[i].velocity();
    }

    // Read Analog inputs 
    for (int i=0; i<constants::NumAnalogInput; i++)
    {
        dev_to_host_msg.analog_input[i] = analogRead(constants::AnalogInputPinArray[i]);
    }

    // Echo back message count  for lag checking 
    dev_to_host_msg.count = host_to_dev_msg_last_.count;

    // Echo back last command and set command response data
    dev_to_host_msg.command = host_to_dev_msg_last_.command;
    dev_to_host_msg.command_data = command_response_data_;
    command_response_data_ =  0;

    return dev_to_host_msg;
}


void SystemState::command_switchyard()
{
    constants::UsbCommand command = constants::UsbCommand(host_to_dev_msg_last_.command);

    switch (command)
    {
        case constants::Cmd_Empty: 
            // Take no action 
            break;

        case constants::Cmd_SetModeDisabled:
            set_mode_disabled();
            break;

        case constants::Cmd_SetModeReady:
            set_mode_ready();
            break;

        case constants::Cmd_SetModeHoming:
            set_mode_homing();
            break;

        case constants::Cmd_SetModePositioning:
            set_mode_positioning();
            break;

        case constants::Cmd_SetModeVelocityControl:
            set_mode_velocity_control();
            break;

        case constants::Cmd_StopMotion:
            stop_motion_cmd();
            break;

        case constants::Cmd_SetAxisHomed:
            set_axis_homed_cmd();
            break;

        case constants::Cmd_GetAxisHomed:
            get_axis_homed_cmd();
            break;

        case constants::Cmd_GetTriggerCount:
            get_trigger_count_cmd();
            break;

        case constants::Cmd_SetTriggerCount:
            set_trigger_count_cmd();
            break;

        case constants::Cmd_GetTriggerEnabled:
            get_trigger_enabled_cmd();
            break;

        case constants::Cmd_SetTriggerEnabled:
            set_trigger_enabled_cmd();
            break;

        case constants::Cmd_SetStepperPosition:
            set_stepper_position_cmd();
            break;

        case constants::Cmd_SetStepperMinPosition:
            set_stepper_min_position_cmd();
            break;

        case constants::Cmd_GetStepperMinPosition:
            get_stepper_min_position_cmd();
            break;

        case constants::Cmd_SetStepperMaxPosition:
            set_stepper_max_position_cmd();
            break;

        case constants::Cmd_GetStepperMaxPosition:
            get_stepper_max_position_cmd();
            break;

        case constants::Cmd_SetStepperMaxSpeed:
            set_stepper_max_speed_cmd();
            break;

        case constants::Cmd_GetStepperMaxSpeed:
            get_stepper_max_speed_cmd();
            break;

        case constants::Cmd_SetStepperMaxAccel:
            set_stepper_max_accel_cmd();
            break;

        case constants::Cmd_GetStepperMaxAccel:
            get_stepper_max_accel_cmd();
            break;

        case constants::Cmd_SetStepperHomingDirection:
            set_stepper_homing_dir_cmd();
            break;

        case constants::Cmd_GetStepperHomingDirection:
            get_stepper_homing_dir_cmd();
            break;

        case constants::Cmd_SaveConfigToEEPROM:
            save_eeprom_config();
            break;

        case constants::Cmd_GetDigitalOutput:
            break;

        default:
            break;

    }
}


void SystemState::set_mode_disabled()
{
    digitalWrite(constants::StepperDriveEnablePin,LOW);
    for (int i=0; i<constants::NumStepper; i++)
    {
        stepper_[i].set_velocity(0);
    }
    mode_ = constants::Mode_Disabled;
}


void SystemState::set_mode_ready(bool ignore_mode)
{
    if ((mode_ == constants::Mode_Disabled) || ignore_mode)
    {
        for (int i=0; i<constants::NumStepper; i++)
        {
            stepper_[i].set_velocity(0);
        }
        estop_monitor_.zero_startup_count();
        digitalWrite(constants::StepperDriveEnablePin,HIGH);
        mode_ = constants::Mode_Ready;
    }
    else
    {
        // Error: you can only issue command to go from Mode_Disabled to Mode_Ready
        // To manually return to Mode_Ready from within a motion mode you need to 
        // issue a stop motion command and wait for motion to stop. The system will
        // then automatically return to ModeReady.  
    }
}

void SystemState::set_mode_homing()
{
   if (mode_ == constants::Mode_Ready)
   {
       uint8_t homing_axis_tmp = uint8_t(host_to_dev_msg_last_.command_data[0]);

       if (homing_axis_tmp < constants::NumStepper)
       {
           homing_axis_ = homing_axis_tmp;
           stepper_[homing_axis_].disable_bounds_check();
           homing_controller_[homing_axis_].reset();
           HomingController::enable();
           mode_ = constants::Mode_Homing;
       }
       else
       {
           // Handle axis out of range error

       }
       
   }
   else
   {
       // Handle incorrect mode error
       // Note: you must be in Mode_Ready before you can enter a motion mode
   }


}


void SystemState::set_mode_positioning()
{
    //if ((mode_ == constants::Mode_Ready) && all_axes_homed())
    if (mode_ == constants::Mode_Ready)
    { 
        // Check to make sure positions are within bounds
        bool bounds_error = false;

        for (int i=0; i<constants::NumStepper; i++)
        {
            if (host_to_dev_msg_last_.stepper_position[i] < stepper_[i].min_position())
            {
                bounds_error = true;
            }
            if (host_to_dev_msg_last_.stepper_position[i] > stepper_[i].max_position())
            {
                bounds_error = true;
            }
        }

        // If OK set position controller set points, reset velocity controllers and enter position mode.
        if (!bounds_error)
        {
            for (int i=0; i<constants::NumStepper; i++)
            {
                position_controller_[i].set_position_setp(host_to_dev_msg_last_.stepper_position[i]);
                stepper_[i].enable_bounds_check();
                velocity_controller_[i].reset();
            }
            mode_ = constants::Mode_Positioning;
        }

    }
    else
    {
       // Error: you must be in Mode_Ready before you can enter a motion mode.
    }

}


void SystemState::set_mode_velocity_control()
{
    if ((mode_ == constants::Mode_Ready) && all_axes_homed())
    {
        for (int i=0; i<constants::NumStepper; i++)
        {
            stepper_[i].enable_bounds_check();
            velocity_controller_[i].reset();
        }
        for (int i=0; i<constants::NumTrigger; i++)
        {
            trigger_[i].set_enabled(true);
        }
        timer_count_ = 0;
        mode_ = constants::Mode_VelocityControl;
    }
    else
    {
       // Error: you must be in Mode_Ready before you can enter a motion mode.
    }
}


void SystemState::stop_motion_cmd()
{
    if ((mode_ == constants::Mode_Positioning) || (mode_ == constants::Mode_VelocityControl))
    {
        for (int i=0; i<constants::NumStepper; i++)
        {
            velocity_controller_[i].set_velocity_setp(0);
        }
    }
    if (mode_ == constants::Mode_Homing)
    {
        HomingController::disable();
        homing_controller_[homing_axis_].set_velocity_setp(0);
        stepper_[homing_axis_].enable_bounds_check();
    }

    if (mode_ == constants::Mode_VelocityControl)
    {
        for (int i=0; i<constants::NumTrigger; i++)
        {
            trigger_[i].set_enabled(false);
        }
    }
    stop_motion_flag_ = true;
}


void SystemState::set_axis_homed_cmd()
{
    uint8_t axis = uint8_t(host_to_dev_msg_last_.command_data[0]);
    if (axis < constants::NumStepper)
    {
        homed_flag_[axis] = true;
    }
    else
    {
        // Error:
    }
}


void SystemState::get_axis_homed_cmd()
{
    uint8_t axis = uint8_t(host_to_dev_msg_last_.command_data[0]);
    if (axis < constants::NumStepper)
    {
        command_response_data_ = homed_flag_[axis];
    }
    else
    {
        // Error:
    }

}


void SystemState::set_trigger_count_cmd()
{
    uint8_t num = uint8_t(host_to_dev_msg_last_.command_data[0]);
    if (num < constants::NumTrigger)
    {
        uint16_t count = host_to_dev_msg_last_.command_data[1];
        trigger_[num].set_count(count);
    }
    else
    {
        // Error:
    }
}


void SystemState::get_trigger_count_cmd()
{
    uint8_t num = uint8_t(host_to_dev_msg_last_.command_data[0]);
    if (num < constants::NumTrigger)
    {
        command_response_data_ = trigger_[num].count();
    }
    else
    {
        // Error:
    }
}


void SystemState::set_trigger_enabled_cmd()
{
    uint8_t num = uint8_t(host_to_dev_msg_last_.command_data[0]);
    if (num < constants::NumTrigger)
    {
        bool value = bool(host_to_dev_msg_last_.command_data[1]);
        trigger_[num].set_enabled(value);
    }
    else
    {
        // Error:
    }
}


void SystemState::get_trigger_enabled_cmd()
{
    uint8_t num = uint8_t(host_to_dev_msg_last_.command_data[0]);
    if (num < constants::NumTrigger)
    {
        command_response_data_ = uint16_t(trigger_[num].enabled());
    }
    else
    {
        // Error: 
    }

}


void SystemState::set_stepper_position_cmd()
{
    uint8_t num = uint8_t(host_to_dev_msg_last_.command_data[0]);
    if (num < constants::NumStepper)
    {
        int32_t position = int32_t(host_to_dev_msg_last_.command_data[1]);
        stepper_[num].set_position(position);
    }
    else
    {
        // Error: 
    }
}


void SystemState::set_stepper_min_position_cmd()
{
    uint8_t num = uint8_t(host_to_dev_msg_last_.command_data[0]);
    if (num < constants::NumStepper)
    {
        int16_t min_position = int16_t(host_to_dev_msg_last_.command_data[1]);
        stepper_[num].set_min_position(min_position);
        velocity_controller_[num].set_min_position(min_position);
    }
    else
    {
        // Error:
    }
}


void SystemState::get_stepper_min_position_cmd()
{
    uint8_t num = uint8_t(host_to_dev_msg_last_.command_data[0]);
    if (num < constants::NumStepper)
    {
        int16_t min_position = int16_t(stepper_[num].min_position());
        command_response_data_ = uint16_t(min_position);
    }
    else
    {
        // Error:
    }
}


void SystemState::set_stepper_max_position_cmd()
{
    uint8_t num = uint8_t(host_to_dev_msg_last_.command_data[0]);
    if (num < constants::NumStepper)
    {
        int16_t max_position = int16_t(host_to_dev_msg_last_.command_data[1]);
        stepper_[num].set_max_position(max_position);
        velocity_controller_[num].set_max_position(max_position);
    }
    else
    {
        // Error:
    }
}


void SystemState::get_stepper_max_position_cmd()
{
    uint8_t num = uint8_t(host_to_dev_msg_last_.command_data[0]);
    if (num < constants::NumStepper)
    {
        int16_t max_position = stepper_[num].max_position();
        command_response_data_ = uint16_t(max_position);
    }
    else
    {
        // Error:
    }
}


void SystemState::set_stepper_max_speed_cmd()
{
    uint8_t num = uint8_t(host_to_dev_msg_last_.command_data[0]);
    if (num < constants::NumStepper)
    {
        uint16_t speed = uint16_t(host_to_dev_msg_last_.command_data[1]);
        stepper_[num].set_max_speed(speed);
        velocity_controller_[num].set_max_speed(speed);
        homing_controller_[num].set_max_speed(speed);
    }
    else
    {
        // Error:
    }
}


void SystemState::get_stepper_max_speed_cmd()
{
    uint8_t num = uint8_t(host_to_dev_msg_last_.command_data[0]);
    if (num < constants::NumStepper)
    {
        uint16_t max_speed = uint16_t(stepper_[num].max_speed());
        command_response_data_ = max_speed;
    }
    else
    {
        // Error:
    }
}


void SystemState::set_stepper_max_accel_cmd()
{
    uint8_t num = uint8_t(host_to_dev_msg_last_.command_data[0]);
    if (num < constants::NumStepper)
    {
        uint16_t max_accel = uint16_t(host_to_dev_msg_last_.command_data[1]);
        velocity_controller_[num].set_max_accel(max_accel);
        homing_controller_[num].set_accel(max_accel);
    }
    else
    {
        // Error:
    }
}


void SystemState::get_stepper_max_accel_cmd()
{
    uint8_t num = uint8_t(host_to_dev_msg_last_.command_data[0]);
    if (num < constants::NumStepper)
    {
        uint16_t max_accel  = uint16_t(velocity_controller_[num].max_accel());
        command_response_data_ = max_accel;
    }
    else
    {
        // Error:
    }
}


void SystemState::set_stepper_homing_dir_cmd()
{
    uint8_t num = uint8_t(host_to_dev_msg_last_.command_data[0]);
    if (num < constants::NumStepper)
    {
       int8_t dir = int8_t(host_to_dev_msg_last_.command_data[1]); 
       homing_controller_[num].set_direction(dir);
    }
    else
    {
        // Error:
    }
}


void SystemState::get_stepper_homing_dir_cmd()
{
    uint8_t num = uint8_t(host_to_dev_msg_last_.command_data[0]);
    if (num < constants::NumStepper)
    {
       int8_t dir = homing_controller_[num].direction(); 
       command_response_data_ = uint16_t(dir);
    }
    else
    {
        // Error:
    }
}


bool SystemState::all_axes_homed()
{
    bool rval = true;
    for (int i =0; i<constants::NumStepper; i++)
    {
        rval &= homed_flag_[i];
    }
    return rval;
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

    for (int i =0; i<constants::NumStepper; i++)
    {
        StepperPin pin = constants::StepperPinArray[i];
        stepper_[i] = Stepper(pin.clk,pin.dir); 
        stepper_[i].initialize();

        stepper_[i].set_min_position(eeprom_data_.stepper_minimum_position[i]);
        stepper_[i].set_max_position(eeprom_data_.stepper_maximum_position[i]);
        stepper_[i].set_max_speed(eeprom_data_.stepper_maximum_speed[i]);

        //stepper_[i].set_min_position(constants::DefaultStepperMinimumPosition[i]);
        //stepper_[i].set_max_position(constants::DefaultStepperMaximumPosition[i]);
        //stepper_[i].set_max_speed(constants::DefaultStepperMaximumSpeed[i]);

        stepper_[i].disable_bounds_check();
        stepper_[i].set_velocity(0);
        homed_flag_[i] = false;
    }
}


void SystemState::setup_velocity_controller()
{
    for (int i=0; i<constants::NumStepper; i++)
    {
        velocity_controller_[i].set_min_position(eeprom_data_.stepper_minimum_position[i]);
        velocity_controller_[i].set_max_position(eeprom_data_.stepper_maximum_position[i]);
        velocity_controller_[i].set_max_speed(eeprom_data_.stepper_maximum_speed[i]);
        velocity_controller_[i].set_max_accel(eeprom_data_.stepper_maximum_accel[i]);

        //velocity_controller_[i].set_min_position(constants::DefaultStepperMinimumPosition[i]);
        //velocity_controller_[i].set_max_position(constants::DefaultStepperMaximumPosition[i]);
        //velocity_controller_[i].set_max_speed(constants::DefaultStepperMaximumSpeed[i]);
        //velocity_controller_[i].set_max_accel(constants::DefaultStepperMaximumAccel[i]);

        velocity_controller_[i].set_velocity(0);
        velocity_controller_[i].set_velocity_setp(0);
        velocity_controller_[i].enable_bounds_check();
        velocity_controller_[i].reset();
    }
}


void SystemState::setup_position_controller()
{
    for (int i=0; i<constants::NumStepper; i++)
    {
        position_controller_[i].set_max_speed(constants::PositionControllerMaximumSpeed[i]);
        position_controller_[i].set_gain(constants::PositionControllerGain[i]);
    }
}


void SystemState::setup_analog_input()
{
    analogReadResolution(constants::AnalogReadNumBits);
    analogReadAveraging(constants::AnalogReadNumAvg);
    analogReference(constants::AnalogRefType);
}


void SystemState::setup_estop_monitor()
{
    estop_monitor_.set_analog_pin(constants::EStopMonitorPin); 
    estop_monitor_.set_threshold(constants::EStopMonitorThreshold);
    estop_monitor_.set_startup_delay(constants::EStopMonitorStartupDelay);
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
        trigger_[i].set_enabled(false);
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
    timer_.begin(timer_callback,constants::TimerPeriod_us);
}


void SystemState::setup_homing()
{
    HomingController::initialize(constants::HomingInterruptPin);

    for (int i=0; i<constants::NumStepper; i++)
    {
        homing_controller_[i] = HomingController(
                eeprom_data_.stepper_homing_direction[i],
                constants::HomingSpeed[i]
                );

        homing_controller_[i].set_max_speed(eeprom_data_.stepper_maximum_speed[i]);
        homing_controller_[i].set_accel(eeprom_data_.stepper_maximum_accel[i]);
    }
}


void SystemState::load_eeprom_config()
{
    EEPROM_Config eeprom_config;
    eeprom_data_ = eeprom_config.get();
    if (eeprom_data_.flag == EEPROM_Config::NotSetFlagValue)
    {
        eeprom_config.set_to_defaults();
    }
    eeprom_data_ = eeprom_config.get();
}


void SystemState::save_eeprom_config()
{
    eeprom_data_.flag = 0;
    for (int i=0; i<constants::NumStepper; i++)
    {
        eeprom_data_.stepper_homing_direction[i] = homing_controller_[i].direction();
        eeprom_data_.stepper_minimum_position[i] = stepper_[i].min_position();
        eeprom_data_.stepper_maximum_position[i] = stepper_[i].max_position();
        eeprom_data_.stepper_maximum_speed[i] = int32_t(stepper_[i].max_speed());
        eeprom_data_.stepper_maximum_accel[i] = int32_t(velocity_controller_[i].max_accel());
    }
    EEPROM_Config eeprom_config;
    eeprom_config.put(eeprom_data_);
}

// SystemState Instance
// ------------------------------------------------------------------------------------------------
SystemState system_state = SystemState();


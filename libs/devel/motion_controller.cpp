#include "motion_controller.hpp"
#include <csignal>
#include <iostream>
#include <bitset>
#define _USE_MATH_DEFINES
#include <cmath>
#include "rawhid_msg_types.h"

bool quit_flag = false;
void sig_int_handler(int param)
{
    quit_flag = true;
}

// MotionController
// --------------------------------------------------------------------------------------
const std::list<Axis> MotionController::AxisList = 
{Axis_X, Axis_Y, Axis_Z, Axis_A, Axis_B, Pwm_0, Pwm_1, Pwm_2, Pwm_3, Pwm_4};

const std::list<Axis> MotionController::StepperList = 
{Axis_X, Axis_Y, Axis_Z, Axis_A, Axis_B }; 

const std::list<Axis> MotionController::PwmList = 
{Pwm_0, Pwm_1, Pwm_2, Pwm_3, Pwm_4 };


MotionController::MotionController(int vid, int pid) 
{
    std::signal(SIGINT,sig_int_handler);

    hid_dev_ = RawHIDDevice(vid,pid);
    for (Axis axis : AxisList)
    {
        disable_homing(axis);
    }

}


bool MotionController::open()
{
    bool rval =  hid_dev_.open();
    return rval;
}


void MotionController::close()
{
    hid_dev_.close();
}


void MotionController::enable_homing(Axis axis)
{
    homing_enabled_map_[axis] = true;
}


void MotionController::disable_homing(Axis axis)
{
    homing_enabled_map_[axis] = false;
}



bool MotionController::is_homing_enabled(Axis axis)
{
    return homing_enabled_map_[axis];
}



void MotionController::test()
{
    int cnt = 0;
    int drop_count = 0;
    uint64_t time_us_last = 0;
    bool rtn_val = false;

    std::cout << "1" << std::endl;
    hid_dev_.clearRecvBuffer();
    std::cout << "2" << std::endl;


    uint64_t time_start_us = 0;
    int32_t position_start[NumStepper];

    while (!quit_flag)
    {
        DevToHostMsg dev_to_host_msg;
        rtn_val = hid_dev_.recvData(&dev_to_host_msg);
        if (!rtn_val)
        {
            std::cerr << "Error: sendData" << std::endl;
            continue;
        }
        else
        {
            uint64_t dt = dev_to_host_msg.time_us - time_us_last;
            time_us_last = dev_to_host_msg.time_us;

            std::cout << "time_us:   " << dev_to_host_msg.time_us  << " " << dt << std::endl;
            std::cout << "dropped:   " << drop_count << "/" << cnt << std::endl;
            std::cout << "status:    " << std::bitset<8>(dev_to_host_msg.status) << std::endl;
            std::cout << "count in:  " << int(dev_to_host_msg.count) << std::endl;

            HostToDevMsg host_to_dev_msg;
            host_to_dev_msg.count = uint8_t(cnt);
            int count_lag = int(host_to_dev_msg.count) - int(dev_to_host_msg.count) - 1;
            if (count_lag > 0)
            {
                drop_count++;
            }
            std::cout << "count out: " << int(host_to_dev_msg.count) << std::endl;
            std::cout << "count lag: " << count_lag  << std::endl;

            for (int i=0; i<NumStepper; i++)
            {
                std::cout << dev_to_host_msg.stepper_position[i] << " ";
            }
            std::cout << std::endl;


            host_to_dev_msg.command = Cmd_Empty;
            
            if (cnt == 5)
            {
                host_to_dev_msg.command = Cmd_SetModeReady;
            }

            if (cnt == 10)
            {
                //host_to_dev_msg.command = Cmd_SetModeHoming;
                //host_to_dev_msg.command_data[0] = Axis_X; 

                //host_to_dev_msg.command = Cmd_SetModePositioning;
                //for (int i=0; i<NumStepper; i++)
                //{
                //    host_to_dev_msg.stepper_position[i] =  2000;
                //}

                host_to_dev_msg.command = Cmd_SetModeVelocityControl;
                for (int i = 0; i<NumStepper; i++)
                {
                    host_to_dev_msg.stepper_velocity[i] = 0;
                    position_start[i] = dev_to_host_msg.stepper_position[i];
                }

                time_start_us = dev_to_host_msg.time_us;

            }
            if (cnt > 10)
            {
                for (int i =0; i<NumStepper; i++)
                {
                    host_to_dev_msg.stepper_velocity[i] = 0;
                }
                float amp = 1000;
                float period = 5.0;
                float t = 1.0e-6*(dev_to_host_msg.time_us - time_start_us);

                float position_setp = position_start[0] +  amp*(1.0 - std::cos(2.0*M_PI*t/period));
                float velocity_setp = (2.0*M_PI*amp/period)*std::sin(2.0*M_PI*t/period);
                float position_curr = float(dev_to_host_msg.stepper_position[0]);
                float error = (position_setp - position_curr);

                float velocity_next = 40.0*error + 0.8*velocity_setp;

                host_to_dev_msg.stepper_velocity[0] = int32_t(velocity_next);

                std::cout << "t: " << t << " pset: " << position_setp << " vset: " << velocity_setp << " vctl: " << velocity_next << std::endl;
                std::cout << "error: " << error << std::endl;

            }

            if (cnt == 5000)
            {
                host_to_dev_msg.command = Cmd_StopMotion; 
            }


            rtn_val = hid_dev_.sendData(&host_to_dev_msg);
            if (!rtn_val)
            {
                std::cerr << "Error: sendData" << std::endl;
            }
            std::cout << std::endl;
            std::cout << std::endl;

        }
        cnt++;
    }
}



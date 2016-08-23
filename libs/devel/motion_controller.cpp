#include "motion_controller.hpp"
#define _USE_MATH_DEFINES
#include <cmath>
#include <csignal>
#include <iostream>
#include <bitset>
#include <chrono>
#include <thread>
#include <algorithm>


namespace motion
{

    // SIGINT handler
    // ------------------------------------------------------------------------------------
    bool quit_flag = false;
    void sig_int_handler(int param)
    {
        quit_flag = true;
    }

    // Controller public methods
    // --------------------------------------------------------------------------------------

    Controller::Controller(int vid, int pid) 
    {
        std::signal(SIGINT,sig_int_handler);

        hid_dev_ = RawHIDDevice(vid,pid);
        for (auto axis : AxisList)
        {
            disable_homing(axis);
        }

    }


    RtnStatus Controller::open()
    {
        RtnStatus rtn_status;
        if (!hid_dev_.open())
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("unable to open device");
        }
        return rtn_status;
    }


    RtnStatus Controller::close()
    {
        hid_dev_.close();
        return RtnStatus();
    }


    RtnStatus Controller::position(std::vector<int32_t> &position)
    {
        RtnStatus rtn_status;
        HostToDevMsg host_to_dev_msg;
        DevToHostMsg dev_to_host_msg;
        host_to_dev_msg.command = Cmd_Empty;
        rtn_status = send_command(host_to_dev_msg, dev_to_host_msg);
        if (rtn_status.success())
        {
            position.resize(NumStepper);
            for (auto num : StepperList)
            {
                position[num] = dev_to_host_msg.stepper_position[num];
            }
        }
        return rtn_status;
    }

    RtnStatus Controller::print_position()
    {
        std::vector<int32_t> pos_vec;
        RtnStatus rtn_status = position(pos_vec);
        for (auto val : pos_vec)
        {
            std::cout << " " << val;
        }
        std::cout << std::endl;
        return rtn_status;
    }

    RtnStatus Controller::set_mode_ready()
    {
        RtnStatus rtn_status;
        HostToDevMsg host_to_dev_msg;
        DevToHostMsg dev_to_host_msg;
        host_to_dev_msg.command = Cmd_SetModeReady;
        rtn_status = send_command(host_to_dev_msg,dev_to_host_msg);
        return rtn_status;
    }


    void Controller::enable_homing(Axis axis)
    {
        homing_enabled_map_[axis] = true;
    }


    void Controller::disable_homing(Axis axis)
    {
        homing_enabled_map_[axis] = false;
    }


    bool Controller::is_homing_enabled(Axis axis)
    {
        return homing_enabled_map_[axis];
    }


    RtnStatus Controller::set_homed_true(Axis axis)
    {
        RtnStatus rtn_status;
        HostToDevMsg host_to_dev_msg;
        DevToHostMsg dev_to_host_msg;
        host_to_dev_msg.command = Cmd_SetAxisHomed;
        host_to_dev_msg.command_data[0] = axis;
        rtn_status = send_command(host_to_dev_msg, dev_to_host_msg);
        return rtn_status;
    }


    RtnStatus Controller::home(Axis axis, bool wait)
    {
        RtnStatus rtn_status;
        HostToDevMsg host_to_dev_msg;
        DevToHostMsg dev_to_host_msg;
        host_to_dev_msg.command = Cmd_SetModeHoming;
        host_to_dev_msg.command_data[0] = axis;
        rtn_status = send_command(host_to_dev_msg, dev_to_host_msg);
        if (rtn_status.success() && wait)
        {
            rtn_status = wait_for_ready();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(HomingDebounceSleep_ms));
        return rtn_status;
    }


    RtnStatus Controller::move_to_position(Axis axis, int32_t pos, bool wait)
    {
        RtnStatus rtn_status;
        std::vector<int32_t> cur_pos_vec;
        rtn_status = position(cur_pos_vec);
        if (rtn_status.success())
        {
            HostToDevMsg host_to_dev_msg;
            DevToHostMsg dev_to_host_msg;
            host_to_dev_msg.command = Cmd_SetModePositioning;
            std::copy(cur_pos_vec.begin(), cur_pos_vec.end(), host_to_dev_msg.stepper_position);
            host_to_dev_msg.stepper_position[axis] = pos;
            rtn_status = send_command(host_to_dev_msg, dev_to_host_msg);
            if (rtn_status.success() && wait)
            {
                rtn_status = wait_for_ready();
            }
        }
        return rtn_status;
    }


    RtnStatus Controller::move_to_position(std::vector<int32_t> pos_vec, bool wait)
    {
        RtnStatus rtn_status;
        if (pos_vec.size() != NumStepper)
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("position vector size must equal NumStepper");
        }
        else
        {
            HostToDevMsg host_to_dev_msg;
            DevToHostMsg dev_to_host_msg;
            host_to_dev_msg.command = Cmd_SetModePositioning;
            std::copy(pos_vec.begin(), pos_vec.end(), host_to_dev_msg.stepper_position);
            rtn_status = send_command(host_to_dev_msg, dev_to_host_msg);
            if (rtn_status.success() && wait)
            {
                rtn_status = wait_for_ready();
            }
        }
        return rtn_status;
    }


    RtnStatus Controller::move_to_position(std::map<Axis,int32_t> pos_map, bool wait)
    {
        RtnStatus rtn_status;
        bool ok = true;
        for (auto kv : pos_map)
        {
            ok &= (std::find(StepperList.begin(), StepperList.end(), kv.first) != StepperList.end());
        }
        if (!ok)
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("axis in not stepper - can't move to position");
        }
        else
        {
            std::vector<int32_t> cur_pos_vec;
            rtn_status = position(cur_pos_vec);
            if (rtn_status.success())
            {
                HostToDevMsg host_to_dev_msg;
                DevToHostMsg dev_to_host_msg;
                host_to_dev_msg.command = Cmd_SetModePositioning;
                std::copy(cur_pos_vec.begin(), cur_pos_vec.end(), host_to_dev_msg.stepper_position);
                for (auto kv : pos_map)
                {
                    host_to_dev_msg.stepper_position[kv.first] = kv.second;
                }
                rtn_status = send_command(host_to_dev_msg, dev_to_host_msg);
                if (rtn_status.success() && wait)
                {
                    rtn_status = wait_for_ready();
                }
            }
        }
        return rtn_status;
    }


    // Private Methods
    // ----------------------------------------------------------------------------------

    RtnStatus Controller::send_command( HostToDevMsg &host_to_dev_msg, DevToHostMsg &dev_to_host_msg)
    {
        RtnStatus rtn_status;

        // Clear buffer to sync messages 
        hid_dev_.clearRecvBuffer();

        // start recv -> send communication pairs.
        if (!hid_dev_.recvData(&dev_to_host_msg))
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("unable to sync messaging loop");
        }
        else
        {
            msg_count_ = dev_to_host_msg.count;
            msg_count_++;

            // Send command to device
            if (!hid_dev_.sendData(&host_to_dev_msg))
            {
                rtn_status.set_success(false);
                rtn_status.set_error_msg("cound not send command to device");
            }
            else
            {
                // Get reponse from device and check - make sure correct command received and check for errors
                if (!hid_dev_.recvData(&dev_to_host_msg))
                {
                    rtn_status.set_success(false);
                    rtn_status.set_error_msg("did not receive response to command");
                }
                else
                {
                    if (dev_to_host_msg.command  != host_to_dev_msg.command)
                    {
                        rtn_status.set_success(false);
                        rtn_status.set_error_msg("response command does not match");
                    }
                }
            }
        }
        return rtn_status;
    }


    RtnStatus Controller::wait_for_ready()
    {
        bool done = false;
        DevToHostMsg dev_to_host_msg;
        HostToDevMsg host_to_dev_msg;
        RtnStatus rtn_status;

        while (!done && !quit_flag)
        {
            // Wait - receiving and sending messages - until mode changes back to ready
            if (!hid_dev_.recvData(&dev_to_host_msg))
            {
                rtn_status.set_success(false);
                rtn_status.set_error_msg("did not receive message from device while in wait loop");
                break;
            }

            host_to_dev_msg.count =  msg_count_;
            host_to_dev_msg.command = Cmd_Empty;
            if (!hid_dev_.sendData(&host_to_dev_msg))
            {
                rtn_status.set_success(false);
                rtn_status.set_error_msg("unable send data to device while in wait loop");
                break;
            }
            msg_count_++;

            // Check to see if done
            OperatingMode mode = get_operating_mode(dev_to_host_msg);
            if (mode  == Mode_Ready)
            {
                done = true;
            }
            if (mode == Mode_Disabled)
            {
                rtn_status.set_success(false);
                rtn_status.set_error_msg("system disabled while in wait loop");
                break;
            }
        }
        return rtn_status;
    }


    void Controller::test()
    {
        int cnt = 0;
        int drop_count = 0;
        uint64_t time_us_last = 0;
        bool rtn_val = false;

        hid_dev_.clearRecvBuffer();

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


    // Utility functions
    // ----------------------------------------------------------------------------------
    OperatingMode get_operating_mode(DevToHostMsg msg)
    {
        uint8_t mask = 0;
        //for (int i=0; i<constants::NumModeBits; i++)
        for (int i=0; i<NumModeBits; i++)
        {
            mask |= (1 << i);
        }
        OperatingMode mode = OperatingMode(msg.status & mask);
        return mode;
    }


    std::string operating_mode_to_string(OperatingMode mode)
    {
        if (OperatingModeToStringMap.count(mode) > 0)
        {
            return OperatingModeToStringMap[mode];
        }
        else
        {
            return std::string("mode not found");
        }
    }


    std::string axis_to_string(Axis axis)
    {
        if (AxisToStringMap.count(axis) > 0)
        {
            return AxisToStringMap[axis];
        }
        else
        {
            return std::string("axis not found");
        }
    }

} // namespace motion



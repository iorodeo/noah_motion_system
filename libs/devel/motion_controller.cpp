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


    RtnStatus Controller::mode(OperatingMode &mode)
    {
        RtnStatus rtn_status;
        HostToDevMsg host_to_dev_msg;
        DevToHostMsg dev_to_host_msg;
        host_to_dev_msg.command = Cmd_Empty;
        rtn_status = send_command(host_to_dev_msg,dev_to_host_msg);
        if (rtn_status.success())
        {
            mode = get_operating_mode(dev_to_host_msg);
        }
        return rtn_status;
    }


    RtnStatus Controller::mode(std::string &mode_str)
    {
        RtnStatus rtn_status;
        OperatingMode op_mode;
        rtn_status = mode(op_mode);
        if (rtn_status.success())
        {
            mode_str = operating_mode_to_string(op_mode);
        }
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


    RtnStatus Controller::set_mode_disabled()
    {
        RtnStatus rtn_status;
        HostToDevMsg host_to_dev_msg;
        DevToHostMsg dev_to_host_msg;
        host_to_dev_msg.command = Cmd_SetModeDisabled;
        rtn_status = send_command(host_to_dev_msg,dev_to_host_msg);
        return rtn_status;
    }


    RtnStatus Controller::position(Axis axis, int32_t &ind)
    {
        RtnStatus rtn_status;
        std::vector<int32_t> ind_vec;
        rtn_status = position(ind_vec);
        if (rtn_status.success())
        {
            ind = ind_vec[axis];
        }
        return rtn_status;
    }


    RtnStatus Controller::position(std::vector<int32_t> &ind_vec)
    {
        RtnStatus rtn_status;
        HostToDevMsg host_to_dev_msg;
        DevToHostMsg dev_to_host_msg;
        host_to_dev_msg.command = Cmd_Empty;
        rtn_status = send_command(host_to_dev_msg, dev_to_host_msg);
        if (rtn_status.success())
        {
            ind_vec = get_index_position_std(dev_to_host_msg);
        }
        return rtn_status;
    }


    RtnStatus Controller::position(std::map<Axis,int32_t> &ind_map)
    {
        RtnStatus rtn_status;
        std::vector<int32_t> ind_vec;
        rtn_status = position(ind_vec);
        if (rtn_status.success())
        {
            ind_map.clear();
            for (int i=0; i<ind_vec.size(); i++)
            {
                std::pair<Axis,int32_t> kv(Axis(i),ind_vec[i]);
                ind_map.insert(kv);
            }
        }
        return rtn_status;
    }


    RtnStatus Controller::position(arma::Row<int32_t> &ind_vec)
    {
        RtnStatus rtn_status;
        HostToDevMsg host_to_dev_msg;
        DevToHostMsg dev_to_host_msg;
        host_to_dev_msg.command = Cmd_Empty;
        rtn_status = send_command(host_to_dev_msg, dev_to_host_msg);
        if (rtn_status.success())
        {
            ind_vec = get_index_position_arma(dev_to_host_msg);
        }
        return rtn_status;
    }


    RtnStatus Controller::position(Axis axis, double  &pos)
    {
        RtnStatus rtn_status;
        std::vector<double> pos_vec;
        rtn_status = position(pos_vec);
        if (rtn_status.success())
        {
            pos = pos_vec[axis];
        }
        return rtn_status;
    }


    RtnStatus Controller::position(std::vector<double>  &pos_vec)
    {
        RtnStatus rtn_status;
        std::vector<int32_t> ind_vec;
        rtn_status = position(ind_vec);
        if (rtn_status.success())
        {
            pos_vec.resize(ind_vec.size());
            pos_vec = config_.index_to_unit(ind_vec);
        }
        return rtn_status;
    }


    RtnStatus Controller::position(std::map<Axis,double> &pos_map)
    {
        RtnStatus rtn_status;
        std::vector<double> pos_vec;
        rtn_status = position(pos_vec);
        if (rtn_status.success())
        {
            pos_map.clear();
            for (int i=0; i<pos_vec.size(); i++)
            {
                std::pair<Axis,double> kv(Axis(i),pos_vec[i]);
                pos_map.insert(kv);
            }
        }
        return rtn_status;
    }


    RtnStatus Controller::position(arma::Row<double> &pos_vec)  
    {
        RtnStatus rtn_status;
        arma::Row<int32_t> ind_vec;
        rtn_status = position(ind_vec);
        if (rtn_status.success())
        {
            pos_vec = config_.index_to_unit(ind_vec);
        }
        return rtn_status;
    }


    RtnStatus Controller::print_position(bool unit)
    {
        RtnStatus rtn_status;
        if (unit)
        {
            std::vector<double> pos_vec;
            rtn_status = position(pos_vec);
            if (rtn_status.success())
            {
                for (auto val : pos_vec)
                {
                    std::cout << " " << val;
                }
                std::cout << std::endl;
            }
        }
        else
        {
            std::vector<int32_t> ind_vec;
            rtn_status = position(ind_vec);
            if (rtn_status.success())
            {
                for (auto val : ind_vec)
                {
                    std::cout << " " << val;
                }
                std::cout << std::endl;
            }
        }
        return rtn_status;
    }



    void Controller::enable_homing(Axis axis)
    {
        config_.set_homing_enabled(axis,true);
    }


    void Controller::disable_homing(Axis axis)
    {
        config_.set_homing_enabled(axis,false);
    }


    bool Controller::is_homing_enabled(Axis axis)
    {
        return config_.homing_enabled(axis);
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


    RtnStatus Controller::home(Axis axis, bool backoff, bool wait)
    {
        RtnStatus rtn_status;
        HostToDevMsg host_to_dev_msg;
        DevToHostMsg dev_to_host_msg;
        host_to_dev_msg.command = Cmd_SetModeHoming;
        host_to_dev_msg.command_data[0] = axis;
        rtn_status = send_command(host_to_dev_msg, dev_to_host_msg);
        if (rtn_status.success() && (wait || backoff))
        {
            rtn_status = wait_for_ready();

            if (rtn_status.success() && backoff)
            {
                double backoff = config_.homing_backoff(axis);
                rtn_status = jog_position(axis,backoff);
                if (rtn_status.success())
                {
                    rtn_status = wait_for_ready();
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(HomingDebounceSleep_ms));
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
            OperatingMode op_mode = get_operating_mode(dev_to_host_msg);
            if (op_mode  == Mode_Ready)
            {
                done = true;
            }
            if (op_mode == Mode_Disabled)
            {
                rtn_status.set_success(false);
                rtn_status.set_error_msg("system disabled while in wait loop");
                break;
            }
        }
        return rtn_status;
    }


    Unit Controller::axis_unit(Axis axis)
    {
        return config_.axis_unit(axis);
    }


    RtnStatus Controller::move_to_position(Axis axis, int32_t ind, bool wait)
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
            host_to_dev_msg.stepper_position[axis] = ind;
            rtn_status = send_command(host_to_dev_msg, dev_to_host_msg);
            if (rtn_status.success() && wait)
            {
                rtn_status = wait_for_ready();
            }
        }
        return rtn_status;
    }


    RtnStatus Controller::move_to_position(std::vector<int32_t> ind_vec, bool wait)
    {
        RtnStatus rtn_status;
        if (ind_vec.size() != NumStepper)
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("position vector size must equal NumStepper");
        }
        else
        {
            HostToDevMsg host_to_dev_msg;
            DevToHostMsg dev_to_host_msg;
            host_to_dev_msg.command = Cmd_SetModePositioning;
            std::copy(ind_vec.begin(), ind_vec.end(), host_to_dev_msg.stepper_position);
            rtn_status = send_command(host_to_dev_msg, dev_to_host_msg);
            if (rtn_status.success() && wait)
            {
                rtn_status = wait_for_ready();
            }
        }
        return rtn_status;
    }


    RtnStatus Controller::move_to_position(std::map<Axis,int32_t> ind_map, bool wait)
    {
        RtnStatus rtn_status;
        bool ok = true;
        for (auto kv : ind_map)
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
            std::vector<int32_t> cur_ind_vec;
            rtn_status = position(cur_ind_vec);
            if (rtn_status.success())
            {
                HostToDevMsg host_to_dev_msg;
                DevToHostMsg dev_to_host_msg;
                host_to_dev_msg.command = Cmd_SetModePositioning;
                std::copy(cur_ind_vec.begin(), cur_ind_vec.end(), host_to_dev_msg.stepper_position);
                for (auto kv : ind_map)
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


    RtnStatus Controller::move_to_position(arma::Row<int32_t> ind_vec, bool wait)
    {
        RtnStatus rtn_status;
        if (ind_vec.size() != NumStepper)
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("position vector size must equal NumStepper");
        }
        else
        {
            HostToDevMsg host_to_dev_msg;
            DevToHostMsg dev_to_host_msg;
            host_to_dev_msg.command = Cmd_SetModePositioning;
            std::copy(ind_vec.begin(), ind_vec.end(), host_to_dev_msg.stepper_position);
            rtn_status = send_command(host_to_dev_msg, dev_to_host_msg);
            if (rtn_status.success() && wait)
            {
                rtn_status = wait_for_ready();
            }
        }
        return rtn_status;
    }


    RtnStatus Controller::move_to_position(Axis axis, double pos, bool wait)
    {
        RtnStatus rtn_status;
        int32_t ind = config_.unit_to_index(axis,pos);
        rtn_status = move_to_position(axis,ind,wait);
        return rtn_status; 
    }


    RtnStatus Controller::move_to_position(std::vector<double> pos_vec, bool wait)
    {
        RtnStatus rtn_status;
        std::vector<int32_t> ind_vec = config_.unit_to_index(pos_vec);
        rtn_status = move_to_position(ind_vec,wait);
        return rtn_status; 
    }


    RtnStatus Controller::move_to_position(std::map<Axis,double> pos_map, bool wait)
    {
        RtnStatus rtn_status;
        std::map<Axis,int32_t> ind_map = config_.unit_to_index(pos_map);
        rtn_status = move_to_position(ind_map,wait);
        return rtn_status; 
    }

    RtnStatus Controller::jog_position(Axis axis, int32_t ind, bool wait)
    {
        RtnStatus rtn_status;
        std::vector<int32_t> jog_vec;
        rtn_status = position(jog_vec);
        if (rtn_status.success())
        {
            jog_vec[axis] = jog_vec[axis] + ind;
            rtn_status = move_to_position(jog_vec,wait);
        }
        return rtn_status;
    }


    RtnStatus Controller::jog_position(std::vector<int32_t> ind_vec, bool wait)
    {
        RtnStatus rtn_status;
        std::vector<int32_t> jog_vec;
        rtn_status = position(jog_vec);
        if (rtn_status.success())
        {
            for (int i=0; i<ind_vec.size(); i++)
            {
                jog_vec[i] = jog_vec[i] + ind_vec[i];
            }
            rtn_status = move_to_position(jog_vec,wait);
        }
        return rtn_status;
    }


    RtnStatus Controller::jog_position(std::map<Axis,int32_t> ind_map, bool wait)
    {
        RtnStatus rtn_status;
        std::map<Axis,int32_t> jog_map;
        rtn_status = position(jog_map);
        if (rtn_status.success())
        {
            for (auto kv : jog_map)
            {
                if (ind_map.count(kv.first) > 0)
                {
                    jog_map[kv.first] = jog_map[kv.first] + kv.second;
                }
            }
            rtn_status = move_to_position(jog_map,wait);
        }
        return rtn_status;
    }


    RtnStatus Controller::jog_position(Axis axis, double pos, bool wait)
    {
        RtnStatus rtn_status;
        int32_t ind = config_.unit_to_index(axis,pos);
        rtn_status = jog_position(axis,ind,wait);
        return rtn_status;
    }


    RtnStatus Controller::jog_position(std::vector<double> pos_vec, bool wait)
    {
        RtnStatus rtn_status;
        std::vector<int32_t> ind_vec = config_.unit_to_index(pos_vec);
        rtn_status = jog_position(ind_vec,wait);
        return rtn_status;
    }


    RtnStatus Controller::jog_position(std::map<Axis,double> pos_map, bool wait)
    {
        RtnStatus rtn_status;
        std::map<Axis,int32_t> ind_map = config_.unit_to_index(pos_map);
        rtn_status = jog_position(ind_map,wait);
        return rtn_status;
    }


    // Protected Methods
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
        OperatingMode mode = OperatingMode(msg.status & ModeBitsMask);
        return mode;
    }

    std::vector<int32_t> get_index_position_std(DevToHostMsg msg)
    {
        std::vector<int32_t> ind_vec(NumStepper);
        for (auto num : StepperList)
        {
            ind_vec[num] = msg.stepper_position[num];
        }
        return ind_vec;
    }
    
    arma::Row<int32_t> get_index_position_arma(DevToHostMsg msg)
    {
        arma::Row<int32_t> ind_vec(NumStepper);
        for (auto num : StepperList)
        {
            ind_vec(num) = msg.stepper_position[num];
        }
        return ind_vec;
    }
    

} // namespace motion



#include "controller.hpp"
#define _USE_MATH_DEFINES
#include <cmath>
#include <csignal>
#include <iostream>
#include <bitset>
#include <chrono>
#include <thread>
#include <algorithm>
#include <iomanip>


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
        return check_status(rtn_status);
    }


    RtnStatus Controller::close()
    {
        hid_dev_.close();
        return RtnStatus();
    }


    bool Controller::exit_on_error()
    {
        return exit_on_error_;
    }


    void Controller::set_exit_on_error(bool value)
    {
        exit_on_error_ = value;
    }


    bool Controller::display_position_on_move()
    {
        return display_position_on_move_;
    }


    void Controller::set_display_position_on_move(bool value)
    {
        display_position_on_move_ = value;
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
        return check_status(rtn_status);
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
        return check_status(rtn_status);
    }


    RtnStatus Controller::set_mode_ready()
    {
        RtnStatus rtn_status;
        HostToDevMsg host_to_dev_msg;
        DevToHostMsg dev_to_host_msg;
        host_to_dev_msg.command = Cmd_SetModeReady;
        rtn_status = send_command(host_to_dev_msg,dev_to_host_msg);
        return check_status(rtn_status);
    }


    RtnStatus Controller::set_mode_disabled()
    {
        RtnStatus rtn_status;
        HostToDevMsg host_to_dev_msg;
        DevToHostMsg dev_to_host_msg;
        host_to_dev_msg.command = Cmd_SetModeDisabled;
        rtn_status = send_command(host_to_dev_msg,dev_to_host_msg);
        return check_status(rtn_status);
    }


    RtnStatus Controller::stop_motion(bool wait, bool check)
    {
        RtnStatus rtn_status;
        HostToDevMsg host_to_dev_msg;
        DevToHostMsg dev_to_host_msg;
        host_to_dev_msg.command = Cmd_StopMotion;
        rtn_status = send_command(host_to_dev_msg,dev_to_host_msg);
        if (wait)
        {
            rtn_status = wait_for_ready(check, true);
        }
        if (check)
        {
            rtn_status = check_status(rtn_status);
        }
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
        return check_status(rtn_status);
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
        return check_status(rtn_status);
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
        return check_status(rtn_status);
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
        return check_status(rtn_status);
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
        return check_status(rtn_status);
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
        return check_status(rtn_status);
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
        return check_status(rtn_status);
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
        return check_status(rtn_status);
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
        return check_status(rtn_status);
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
        return check_status(rtn_status);
    }


    RtnStatus Controller::is_homed(Axis axis, bool &is_homed_flag)
    {
        RtnStatus rtn_status;
        HostToDevMsg host_to_dev_msg;
        DevToHostMsg dev_to_host_msg;
        host_to_dev_msg.command = Cmd_GetAxisHomed;
        host_to_dev_msg.command_data[0] = axis;
        rtn_status = send_command(host_to_dev_msg, dev_to_host_msg);
        if (rtn_status.success())
        {
            is_homed_flag = (dev_to_host_msg.command_data != 0);
        }
        return check_status(rtn_status);
    }


    RtnStatus Controller::is_homed(std::map<Axis,bool> &is_homed_map)
    {
        RtnStatus rtn_status;
        is_homed_map.clear();
        for (auto num : StepperList)
        {
            bool is_homed_flag;
            rtn_status = is_homed(Axis(num),is_homed_flag);
            if (!rtn_status.success())
            {
                break;
            }
            is_homed_map.insert(std::pair<Axis,bool>(Axis(num), is_homed_flag));
        }
        return check_status(rtn_status);
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
        return check_status(rtn_status);
    }


    RtnStatus Controller::wait_for_ready(bool check, bool quiet)
    {
        bool done = false;
        DevToHostMsg dev_to_host_msg;
        HostToDevMsg host_to_dev_msg;
        RtnStatus rtn_status;

        std::streamsize original_precision = std::cout.precision();
        if (display_position_on_move_ && !quiet)
        {
            std::cout << std::setprecision(4);
            std::cout << std::fixed;
            std::cout << std::endl;
        }

        while (!done && !quit_flag)
        {
            // Wait - receiving and sending messages - until mode changes back to ready
            if (!hid_dev_.recvData(&dev_to_host_msg))
            {
                rtn_status.set_success(false);
                rtn_status.set_error_msg("did not receive message from device while in wait loop");
                break;
            }

            host_to_dev_msg.command = Cmd_Empty;
            host_to_dev_msg.count = msg_count_;
            msg_count_++;
            if (!hid_dev_.sendData(&host_to_dev_msg))
            {
                rtn_status.set_success(false);
                rtn_status.set_error_msg("unable send data to device while in wait loop");
                break;
            }


            // Display position data
            if (display_position_on_move_ && !quiet)
            {
                arma::Row<int32_t> ind_vec = get_index_position_arma(dev_to_host_msg);
                arma::Row<double>  pos_vec = config_.index_to_unit(ind_vec);
                std::cout << '\r';
                for (auto pos : pos_vec)
                {
                    std::cout << std::setw(10) << pos; 
                }
                std::cout << std::flush;
            }

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

        if (display_position_on_move_ && !quiet)
        {
            std::cout << std::setprecision(original_precision);
            std::cout << std::endl << std::endl;
        }

        if (check)
        {
            rtn_status = check_status(rtn_status);
        }
        return rtn_status;
    }


    Unit Controller::axis_unit(Axis axis)
    {
        return config_.axis_unit(axis);
    }

    std::string Controller::axis_unit_string(Axis axis)
    {
        return config_.axis_unit_string(axis);
    }


    std::string Controller::axis_name(Axis axis)
    {
        return config_.axis_name(axis);
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
        return check_status(rtn_status);
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
        return check_status(rtn_status);
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
        return check_status(rtn_status);
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
        return check_status(rtn_status);
    }


    RtnStatus Controller::move_to_position(Axis axis, double pos, bool wait)
    {
        RtnStatus rtn_status;
        int32_t ind = config_.unit_to_index(axis,pos);
        rtn_status = move_to_position(axis,ind,wait);
        return check_status(rtn_status); 
    }


    RtnStatus Controller::move_to_position(std::vector<double> pos_vec, bool wait)
    {
        RtnStatus rtn_status;
        std::vector<int32_t> ind_vec = config_.unit_to_index(pos_vec);
        rtn_status = move_to_position(ind_vec,wait);
        return check_status(rtn_status); 
    }


    RtnStatus Controller::move_to_position(std::map<Axis,double> pos_map, bool wait)
    {
        RtnStatus rtn_status;
        std::map<Axis,int32_t> ind_map = config_.unit_to_index(pos_map);
        rtn_status = move_to_position(ind_map,wait);
        return check_status(rtn_status); 
    }

    RtnStatus Controller::move_to_position(arma::Row<double> pos_vec, bool wait) 
    {
        RtnStatus rtn_status;
        arma::Row<int32_t> ind_vec = config_.unit_to_index(pos_vec);
        rtn_status = move_to_position(ind_vec,wait);
        return check_status(rtn_status);
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
        return check_status(rtn_status);
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
        return check_status(rtn_status);
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
        return check_status(rtn_status);
    }

    RtnStatus Controller::jog_position(arma::Row<int32_t> ind_vec, bool wait) 
    {
        RtnStatus rtn_status;
        arma::Row<int32_t> jog_vec;
        rtn_status = position(jog_vec);
        if (rtn_status.success())
        {
            jog_vec = jog_vec + ind_vec;
            rtn_status = move_to_position(jog_vec,wait);
        }

        return check_status(rtn_status);
    }


    RtnStatus Controller::jog_position(Axis axis, double pos, bool wait)
    {
        RtnStatus rtn_status;
        int32_t ind = config_.unit_to_index(axis,pos);
        rtn_status = jog_position(axis,ind,wait);
        return check_status(rtn_status);
    }


    RtnStatus Controller::jog_position(std::vector<double> pos_vec, bool wait)
    {
        RtnStatus rtn_status;
        std::vector<int32_t> ind_vec = config_.unit_to_index(pos_vec);
        rtn_status = jog_position(ind_vec,wait);
        return check_status(rtn_status);
    }


    RtnStatus Controller::jog_position(std::map<Axis,double> pos_map, bool wait)
    {
        RtnStatus rtn_status;
        std::map<Axis,int32_t> ind_map = config_.unit_to_index(pos_map);
        rtn_status = jog_position(ind_map,wait);
        return check_status(rtn_status);
    }

    RtnStatus Controller::jog_position(arma::Row<double> pos_vec, bool wait)
    {
        RtnStatus rtn_status;
        arma::Row<int32_t> ind_vec = config_.unit_to_index(pos_vec);
        rtn_status = jog_position(ind_vec,wait);
        return check_status(rtn_status);
    }


    RtnStatus Controller::outscan(
            arma::Mat<int32_t> ind_pos_mat, 
            arma::Mat<int32_t> ind_vel_mat, 
            OutscanData &data,
            bool quiet
            )
    {
        RtnStatus rtn_status;

        // Check position and velocity matrix sizes.
        if ((ind_pos_mat.n_cols != NumStepper) || (ind_vel_mat.n_cols != NumStepper))
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("matrices must have #columns == NumStepper");
            return check_status(rtn_status);
        }
        if (ind_pos_mat.n_rows != ind_vel_mat.n_rows)
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("position and velocity matrices must have the same number of rows");
            return check_status(rtn_status);
        }


        // Move to beginning of trajectory
        if (!quiet)
        {
            std::cout << std::endl;
            std::cout << "move to start" << std::endl;
            rtn_status = move_to_position(ind_pos_mat.row(0),true);
        }
        if (!rtn_status.success())
        {
            return check_status(rtn_status);

        }

        // Wait for outscan delay period before proceeding
        if (config_.outscan_start_delay() > 0.0)
        {
            if (!quiet)
            {
                std::cout << "outscan start delay ... " << std::flush;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(config_.outscan_start_delay()));
            if (!quiet)
            {
                std::cout << "done" << std::endl;
            }
        }

        // Setup display output
        std::streamsize original_precision = std::cout.precision();
        if (display_position_on_move_)
        {
            std::cout << std::setprecision(4);
            std::cout << std::fixed;
            std::cout << std::endl;
        }

        DevToHostMsg dev_to_host_msg;
        HostToDevMsg host_to_dev_msg;

        if (!quiet)
        {
            std::cout << "outscan trajectory" << std::endl << std::endl;
        }

        // Enable velocity control mode
        rtn_status = set_mode_velocity_control();
        if (!rtn_status.success())
        {
            return check_status(rtn_status);

        }

        // Outscan trajectory
        for (int i=1; i<ind_pos_mat.n_rows; i++)
        {
            // Receive data from device
            if (!hid_dev_.recvData(&dev_to_host_msg))
            {
                rtn_status.set_success(false);
                rtn_status.set_error_msg("unable to sync messaging loop");
                break;
            }

            // On first receive check to ensure device recieved set velocity mode command.
            if (i==1)
            {
                if (dev_to_host_msg.command != Cmd_SetModeVelocityControl)
                {
                    rtn_status.set_success(false);
                    rtn_status.set_error_msg("failed to receive Cmd_SetModeVelocityControl");
                    break;
                }
            }

            // Get controller velocity correction using tracking error. 
            arma::Row<int32_t> traj_next = ind_pos_mat.row(i);            
            arma::Row<int32_t> velo_next = ind_vel_mat.row(i);
            arma::Row<int32_t> axis_curr = get_index_position_arma(dev_to_host_msg);
            arma::Row<int32_t> error = traj_next - axis_curr;
            arma::Row<int32_t> velo_ctlr = config_.gain()*error + velo_next;

            // Create host to dev message and send to device
            host_to_dev_msg.command = Cmd_Empty;
            host_to_dev_msg.count = msg_count_;
            msg_count_++;
            for (auto num : StepperList)
            {
                host_to_dev_msg.stepper_velocity[num] = velo_ctlr(num);
            }
            if (!hid_dev_.sendData(&host_to_dev_msg))
            {
                rtn_status.set_success(false);
                rtn_status.set_error_msg("unable to sync messaging loop");
                break;
            }

            // Update Outscan data
            data.update(dev_to_host_msg);

            // Display position
            if (display_position_on_move_ && !quiet)
            {
                arma::Row<double>  pos_vec = config_.index_to_unit(axis_curr);
                std::cout << '\r';
                for (auto pos : pos_vec)
                {
                    std::cout << std::setw(10) << pos; 
                }
                std::cout << std::flush;
            }

            if (quit_flag)
            {
                break;
            }

        } // for (int i

        // Restore cout settings
        if (display_position_on_move_ && !quiet)
        {
            std::cout << std::setprecision(original_precision);
            std::cout << std::endl << std::endl;
        }

        // Stop velocity control mode 
        rtn_status = stop_motion(true,true);

        return check_status(rtn_status);
    }


    RtnStatus Controller::outscan(arma::Mat<double> pos_mat, OutscanData &data, bool quiet)   
    {
        RtnStatus rtn_status;

        // Check size of position matrix
        if (pos_mat.n_cols != NumStepper)
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("position matrix size incorrent, n_cols != NumStepper");
            return check_status(rtn_status);
        }
        if (pos_mat.n_rows < 3)
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("position matrix size incorrect, n_row < 3");
            return check_status(rtn_status);
        }

        // Compute velocity matrix via central difference
        int n_rows = pos_mat.n_rows;
        int n_cols = pos_mat.n_cols;
        arma::Mat<double> vel_mat(n_rows,n_cols);

        double dt = MessagePeriod_us*1.0e-6;

        vel_mat.row(0) = arma::Row<double>(n_cols,arma::fill::zeros);
        vel_mat.row(n_rows-1) = arma::Row<double>(n_cols,arma::fill::zeros);
       
        arma::Mat<double> pos_mat_t0 = pos_mat.submat(0,0,n_rows-3,n_cols-1);
        arma::Mat<double> pos_mat_t1 = pos_mat.submat(2,0,n_rows-1,n_cols-1);
        vel_mat.submat(1,0,n_rows-2,n_cols-1) = (pos_mat_t1 - pos_mat_t0)/(2.0*dt);

        // Convert to indices
        arma::Mat<int32_t> ind_pos_mat = config_.unit_to_index(pos_mat);
        arma::Mat<int32_t> ind_vel_mat = config_.unit_to_index(vel_mat);

        // Outscan trajectory
        rtn_status = outscan(ind_pos_mat,ind_vel_mat,data,quiet);
        return check_status(rtn_status);
    }


    RtnStatus Controller::outscan(std::string filename, OutscanData &data, bool quiet)
    {
        RtnStatus rtn_status;
        arma::Mat<double> pos_mat;
        pos_mat.load(filename,arma::raw_ascii);
        if (pos_mat.n_elem  == 0)
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("loading outscan trajectory matrix failed - zero elements loaded");
            return check_status(rtn_status);
        }
        rtn_status = outscan(pos_mat,data,quiet);
        return check_status(rtn_status);
    }


    RtnStatus Controller::outscan(const char filename[], OutscanData &data, bool quiet)
    {
        return outscan(std::string(filename),data, quiet);
    }


    Configuration Controller::config()
    {
        return config_;
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


    
    RtnStatus Controller::check_status(RtnStatus rtn_status)
    {
        rtn_status.set_user_quit(quit_flag);
        if (exit_on_error_ && (!rtn_status.success() || rtn_status.user_quit()))
        {
            if (!rtn_status.success())
            {
                std::cerr << "error: " << rtn_status.error_msg() << std::endl;
            }
            if (rtn_status.user_quit())
            {
                std::cout << std::endl;
                std::cout << "(SIGINT) user quit, stopping ... " << std::flush;
                stop_motion(true,false);
                std::cout << "done" << std::endl << std::endl;
            }
            close();
            exit(0);
        }
        return rtn_status;
    }


    RtnStatus Controller::set_mode_velocity_control()
    {
        RtnStatus rtn_status;
        HostToDevMsg host_to_dev_msg;
        DevToHostMsg dev_to_host_msg;
        host_to_dev_msg.command = Cmd_SetModeVelocityControl;
        for (auto num : StepperList)
        {
            host_to_dev_msg.stepper_velocity[num] = 0;
        }

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
        }
        return check_status(rtn_status);
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


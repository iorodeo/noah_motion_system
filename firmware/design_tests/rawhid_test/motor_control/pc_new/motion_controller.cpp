#include "motion_controller.hpp"
#include <iostream>
#include <fstream>
#include <cstring>
#include <csignal>
#include <iomanip>
#include "msg_types.h"
#include "constants.h"

extern bool quit_flag;


const std::string  MotionController::Default_Output_Filename("data.txt");

MotionController::MotionController(int vid, int pid)
{
    hid_dev_ = RawHIDDevice(constants::Vid,constants::Pid);
    output_filename_ = Default_Output_Filename;
}


bool MotionController::open()
{
    bool rval =  hid_dev_.open();
    if (!rval)
    {
        std::cerr << "error: unable to open device" << std::endl;
    }
    return rval;
}


bool MotionController::close()
{
    hid_dev_.close();
    return true;
}


bool MotionController::run()
{
    // Set to trejctory start position and zero velocity
    if (!set_pos_to_start()) 
    { 
        return false; 
    }
    if (!set_vel_to_zero())  
    { 
        return false; 
    }

    // Set to velocity mode
    if (!set_to_vel_mode())  
    { 
        return false; 
    }


    // Loop over trajectory until done
    std::ofstream outfile;
    uint64_t cnt = 0;

    outfile.open(output_filename_);

    while (true)
    {
        // Receive position message
        PosMsg pos_msg;
        if (!hid_dev_.recvData(&pos_msg))
        {
            std::cerr << "error: recvData failure" << std::endl;
            if (quit_flag)
            {
                break;
            }
            continue;
        }
        if (pos_msg.msg_id != constants::PosMsgId)
        {
            std::cerr << "error: recvData expected PosMsgId" << std::endl;
            if (quit_flag)
            { 
                break;
            }
            continue;
        }
        if (pos_msg.quit_flag)
        {
            break;
        }

        // Compute new motor velocities and send controller
        double t = 1.0e-6*float(pos_msg.time);
        uint32_t index = uint32_t(cnt%trajectory_.size());

        VelMsg vel_msg;
        vel_msg.msg_id = constants::VelMsgId;
        vel_msg.quit_flag = quit_flag;

        outfile << t << " ";

        for (int i=0; i<constants::NumMotor; i++)
        {
            int32_t pos_error = int32_t(trajectory_[index].pos[i]) - pos_msg.position[i]; 
            vel_msg.velocity[i] = (constants::PGain/constants::Dt)*float(pos_error); 
            vel_msg.velocity[i] +=  constants::FGain*trajectory_[index].vel[i]; 
            outfile << int32_t(trajectory_[index].pos[i]) << " " << pos_msg.position[i] << " ";
        }
        outfile << std::endl;

        std::cout << std::fixed << std::showpoint << std::setprecision(5);
        std::cout << t << " " << pos_msg.position[0] << std::endl;

        if (!hid_dev_.sendData(&vel_msg))
        {
            std::cerr << "error: sendData" << std::endl;
        }
        cnt++;
    }

    // Set to trejctory start position and zero velocity
    set_vel_to_zero();  
    set_pos_to_start(); 

    outfile.close();
    return true;
}


void MotionController::set_output_filename(std::string filename)
{
    output_filename_ = filename;
}


std::string MotionController::get_output_filename()
{
    return output_filename_;
}


void MotionController::set_trajectory(std::vector<TrajData> trajectory)
{
    trajectory_ = trajectory;
}


std::vector<TrajData> MotionController::get_trajectory()
{
    return trajectory_;
}


// Protected methods
// ----------------------------------------------------------------------------
bool MotionController::set_pos_to_start()
{
    CmdMsg cmd_msg;
    cmd_msg.msg_id = constants::CmdMsgId;
    cmd_msg.cmd_id = constants::CmdSetMotorPos;
    int32_t pos[constants::NumMotor];
    for (int i=0; i<constants::NumMotor; i++)
    {
        pos[i] = int32_t(trajectory_[0].pos[i]);
    }
    std::memcpy(cmd_msg.data,pos,sizeof(pos));

    bool rval = true;
    if (!hid_dev_.sendData(&cmd_msg))
    {
        std::cerr << "error: unable to set initial motor position" << std::endl;
        rval = false;
    }
    return rval;
}


bool MotionController::set_vel_to_zero()
{
    CmdMsg cmd_msg;
    cmd_msg.msg_id = constants::CmdMsgId;
    cmd_msg.cmd_id = constants::CmdSetMotorVel;
    float vel[constants::NumMotor];
    for (int i=0; i<constants::NumMotor; i++)
    {
        vel[i] = trajectory_[0].vel[i];
    }
    std::memcpy(cmd_msg.data,vel,sizeof(vel));

    bool rval = true;
    if (!hid_dev_.sendData(&cmd_msg))
    {
        std::cerr << "error: unable to set initial motor velocity" << std::endl;
        rval = false;
    }
    return rval;
}


bool MotionController::set_to_vel_mode()
{
    CmdMsg cmd_msg;
    cmd_msg.msg_id = constants::CmdMsgId;
    cmd_msg.cmd_id = constants::CmdStartVelCtrl;

    bool rval = true;
    if (!hid_dev_.sendData(&cmd_msg))
    {
        std::cerr << "error: unable to enter velocity control mode" << std::endl;
        rval = false;
    }
    return rval;
}

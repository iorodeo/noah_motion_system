/*
Copyright 2016 IO Rodeo Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <iostream>
#include <fstream>
#include <csignal>
#include <iomanip>
#include <cstring>
#include "msg_types.h"
#include "rawhid_device.hpp"
#include "trajectory.hpp"

bool quit_flag = false;
void sig_int_handler(int param);


int main(int argc, char *argv[])
{
    std::ofstream outfile;
    std::vector<TrajData> traj = get_cosine_traj(constants::Dt);
    std::signal(SIGINT,sig_int_handler);
    CmdMsg cmd_msg;

    RawHIDDevice dev = RawHIDDevice(constants::Vid,constants::Pid);
    dev.setTimeout(constants::RawHidTimeout);
    if (!dev.open())
    {
        std::cerr << "error: unable to open device" << std::endl;
        return -1;
    }

    // Set initial motor position
    // ------------------------------------------------------------------------
    cmd_msg.msg_id = constants::CmdMsgId;
    cmd_msg.cmd_id = constants::CmdSetMotorPos;
    int32_t pos[constants::NumMotor];
    for (int i=0; i<constants::NumMotor; i++)
    {
        pos[i] = int32_t(traj[0].pos[i]);
    }
    std::memcpy(cmd_msg.data,pos,sizeof(pos));
    if (!dev.sendData(&cmd_msg))
    {
        std::cerr << "error: unable to set initial motor position" << std::endl;
        return -1;
    }
    

    // Set initial motor velocity
    // ------------------------------------------------------------------------
    cmd_msg.msg_id = constants::CmdMsgId;
    cmd_msg.cmd_id = constants::CmdSetMotorVel;
    float vel[constants::NumMotor];
    for (int i=0; i<constants::NumMotor; i++)
    {
        vel[i] = traj[0].vel[i];
    }
    std::memcpy(cmd_msg.data,vel,sizeof(vel));
    if (!dev.sendData(&cmd_msg))
    {
        std::cerr << "error: unable to set initial motor velocity" << std::endl;
        return -1;
    }

    // Enter velocity control mode and outscan trajectory
    // ------------------------------------------------------------------------
    cmd_msg.msg_id = constants::CmdMsgId;
    cmd_msg.cmd_id = constants::CmdStartVelCtrl;
    if (!dev.sendData(&cmd_msg))
    {
        std::cerr << "error: unable to enter velocity control mode" << std::endl;
        return -1;
    }

    uint64_t cnt = 0;
    outfile.open("data.txt");

    while (1)
    {
        // Receive position message
        PosMsg pos_msg;
        if (!dev.recvData(&pos_msg))
        {
            continue;
        }
        if (pos_msg.msg_id != constants::PosMsgId)
        {
            std::cerr << "error: recvData expected PosMsgId" << std::endl;
            continue;
        }
        if (pos_msg.quit_flag)
        {
            break;
        }

        // Compute new velocities and send
        double t = 1.0e-6*float(pos_msg.time);
        uint32_t index = uint32_t(cnt%traj.size());

        VelMsg vel_msg;
        vel_msg.msg_id = constants::VelMsgId;
        vel_msg.quit_flag = quit_flag;

        outfile << t << " ";

        for (int i=0; i<constants::NumMotor; i++)
        {
            int32_t pos_error = int32_t(traj[index].pos[i]) - pos_msg.position[i]; 
            vel_msg.velocity[i] = (constants::PGain/constants::Dt)*float(pos_error); 
            vel_msg.velocity[i] +=  constants::FGain*traj[index].vel[i]; 
            outfile << int32_t(traj[index].pos[i]) << " " << pos_msg.position[i] << " ";
        }
        outfile << std::endl;

        std::cout << std::fixed << std::showpoint << std::setprecision(5);
        std::cout << t << " " << pos_msg.position[0] << std::endl;

        if (!dev.sendData(&vel_msg))
        {
            std::cerr << "error: sendData" << std::endl;
        }

        cnt++;
    }

    // Set final motor velocity
    // ------------------------------------------------------------------------
    cmd_msg.msg_id = constants::CmdMsgId;
    cmd_msg.cmd_id = constants::CmdSetMotorVel;
    vel[constants::NumMotor];
    for (int i=0; i<constants::NumMotor; i++)
    {
        vel[i] = 0.0;
    }
    std::memcpy(cmd_msg.data,vel,sizeof(vel));
    if (!dev.sendData(&cmd_msg))
    {
        std::cerr << "error: unable to set final motor velocity" << std::endl;
    }

    // Set final motor position
    // ------------------------------------------------------------------------
    cmd_msg.msg_id = constants::CmdMsgId;
    cmd_msg.cmd_id = constants::CmdSetMotorPos;
    pos[constants::NumMotor];
    for (int i=0; i<constants::NumMotor; i++)
    {
        pos[i] = 10*i;
    }
    std::memcpy(cmd_msg.data,pos,sizeof(pos));
    if (!dev.sendData(&cmd_msg))
    {
        std::cerr << "error: unable to set final motor position" << std::endl;
    }

    outfile.close();
    dev.close();
    return 0;
}

void sig_int_handler(int param)
{
    quit_flag = true;
}

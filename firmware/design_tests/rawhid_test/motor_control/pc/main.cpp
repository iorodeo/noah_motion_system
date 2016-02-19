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
#include <cmath>
#include <csignal>
#include <iomanip>
#include "rawhid_device.hpp"

const int NumMotor = 8;
const int CmdIdSetVelocity = 1;
const double Amplitude = 1000.0;
const double Period = 8.0;
const double StepSize = 0.005;
const double pgain = 0.1;
const double fgain = 1.0;


bool quit_flag = false;

struct CmdMsg
{
    uint16_t id;
    float velocity[NumMotor];
    uint8_t padding[26];
};

struct DevMsg
{
    uint64_t time;
    int32_t position[NumMotor];
    uint8_t padding[24];
};


struct TrajData
{
    double pos;
    double vel;
};

std::vector<TrajData> get_cosine_traj(double amplitude, double period, double dt);
void sig_int_handler(int param);


int main(int argc, char *argv[])
{
    static uint64_t time_last = 0; 
    int vid = 0x16C0;
    int pid = 0x0486;
    std::vector<TrajData> traj = get_cosine_traj(Amplitude,Period,StepSize);
    bool rval;

    CmdMsg cmd_msg;
    DevMsg dev_msg;

    for (int j=0; j<NumMotor;j++)
    {
        cmd_msg.velocity[j] = 0;
    }

    std::signal(SIGINT,sig_int_handler);
        
    RawHIDDevice dev = RawHIDDevice(vid,pid);
    dev.setTimeout(100);

    rval = dev.open();
    if (!rval)
    {
        std::cout << "unable to open device" << std::endl;
        return -1;
    }
    //std::cout << "device opened" << std::endl;

    uint64_t cnt = 0;
    double t = 0.0;
    int32_t pos_offset = 0;
    while (!quit_flag)
    {
        // Recive data
        // -------------------------------------------------------------------
        rval = dev.recvData(&dev_msg);
        if (rval)
        {
            uint64_t dt_us = dev_msg.time - time_last;
            time_last = dev_msg.time;

            // Send data
            // --------------------------------------------------------------------
            uint32_t index = uint32_t(cnt % traj.size());
            int32_t pos_curr = dev_msg.position[0];
            int32_t pos_next = int32_t(traj[index].pos);
            if (cnt == 0)
            {
                pos_offset = pos_next -  pos_curr;
            }
            int32_t pos_error = pos_next - pos_curr - pos_offset;
            float velocity = (pgain/StepSize)*float(pos_error) + fgain*traj[index].vel; 

            cmd_msg.id = CmdIdSetVelocity;
            cmd_msg.velocity[0] = velocity;
            rval = dev.sendData(&cmd_msg);

            int wint = 7;
            int wdbl = 15;
            std::cout << std::fixed << std::showpoint << std::setprecision(5);
            std::cout << t; 
            std::cout << std::setw(wint) << (pos_curr+pos_offset); 
            std::cout << std::setw(wint) << pos_next; 
            std::cout << std::setw(wint) << pos_error; 
            std::cout << std::endl;

            cnt++;
            t += StepSize;
        }
    }

    for (int i=0; i<4; i++)
    {
        rval = dev.recvData(&dev_msg);
        cmd_msg.id = CmdIdSetVelocity;
        cmd_msg.velocity[0] = 0;
        rval = dev.sendData(&cmd_msg);
    }

    dev.close();
    //std::cout << "device closed" << std::endl;
    return 0;
}


std::vector<TrajData> get_cosine_traj(double amplitude, double period, double dt)
{
    const double pi = std::acos(-1);
    double t = 0.0;
    std::vector<TrajData> traj;
    while (t < period)
    {
        TrajData traj_data;
        traj_data.pos = amplitude*std::cos(2.0*pi*t/period);
        traj_data.vel = -((2.0*pi*amplitude)/period)*std::sin(2.0*pi*t/period);
        traj.push_back(traj_data);
        t += dt;
    }
    return traj;
}

void sig_int_handler(int param)
{
    quit_flag = true;
}

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
#include "rawhid_device.hpp"
#include "joystick.hpp"
#include "dynamic_model.hpp"

#include <unistd.h>
#include <iostream>
#include <cmath>
#include <csignal>
#include <limits>
#include <iomanip>

const int32_t Int16Max = std::numeric_limits<int16_t>::max();

const int vid = 0x16C0;
const int pid = 0x0486;

const int NumMotor = 8;
const int CmdIdSetVelocity = 1;
const double pgain = 0.075;
const double fgain = 1.0;

const double dt = 0.005;
const double mass = 10.0;
const double damping = 3.0;
const double springk = 0.0;
const double spring0 = 0.0;
const double force_const = 5000.0;


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

void sig_int_handler(int param);


int main(int argc, char *argv[])
{
    static uint64_t time_last = 0; 
    CmdMsg cmd_msg;
    DevMsg dev_msg;
    Joystick joystick(0);
    DynamicModel model(mass, damping, springk, spring0);
    model.initialize(0.0,0.0);

    // Ensure that it was found and that we can use it
    if (!joystick.isFound())
    {
        std::cout << "no joystick found" << std::endl;
        exit(1);
    }

    for (int j=0; j<NumMotor;j++)
    {
        cmd_msg.velocity[j] = 0;
    }

    std::signal(SIGINT,sig_int_handler);
        
    RawHIDDevice dev = RawHIDDevice(vid,pid);
    dev.setTimeout(100);

    bool rval = dev.open();
    if (!rval)
    {
        std::cout << "unable to open device" << std::endl;
        return -1;
    }
    //std::cout << "device opened" << std::endl;

    uint64_t cnt = 0;
    double t = 0.0;
    double force = 0.0;
    while (!quit_flag)
    {
        // Recive data
        // -------------------------------------------------------------------
        rval = dev.recvData(&dev_msg);
        if (rval)
        {
            // Timer dt. 
            uint64_t dt_timer_us = dev_msg.time - time_last;
            time_last = dev_msg.time;

            if (cnt==0)
            {
                model.set_position(double(dev_msg.position[0]));
            }

            // Get joystick data
            JoystickEvent event;
            if (joystick.sample(&event))
            {
                if (event.isAxis() && (event.number==1))
                {
                    double value = double(event.value)/double(Int16Max);
                    force = force_const*value;
                }
            }
            model.update(force,dt);

            // Send data
            // --------------------------------------------------------------------
            int32_t pos_curr = dev_msg.position[0];
            int32_t pos_next = int32_t(model.position());
            int32_t pos_error = pos_next - pos_curr;
            //std::cout << model.velocity() << " " << int32_t(model.position()) << " " << pos_curr << std::endl;
            float velocity = (pgain/dt)*float(pos_error) + fgain*model.velocity(); 

            cmd_msg.id = CmdIdSetVelocity;
            cmd_msg.velocity[0] = velocity;
            rval = dev.sendData(&cmd_msg);

            int wint = 7;
            int wdbl = 15;
            std::cout << std::fixed << std::showpoint << std::setprecision(5);
            std::cout  << t; 
            std::cout << std::setw(wdbl) << force;
            std::cout << std::setw(wdbl) << velocity; 
            std::cout << std::setw(wdbl) << model.velocity();
            std::cout << std::setw(wint) << pos_curr; 
            std::cout << std::setw(wint) << pos_next; 
            std::cout << std::setw(wint) << pos_error; 
            std::cout << std::endl;

            cnt++;
            t += dt;
        }
    }

    // Why do I need to send more than one???
    for (int i=0; i<10; i++)
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


void sig_int_handler(int param)
{
    quit_flag = true;
}

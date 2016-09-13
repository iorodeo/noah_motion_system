#include <iostream>
#include <csignal>
#include <cstdlib>
#include "rawhid_device.hpp"

const int Vid = 0x16C0;
const int Pid = 0x0486;
const int NumStepper = 5;
const int NumAnalogInput = 10;
const int NumPwm = 5;

struct __attribute__((packed)) DevToHostMsg 
{
    uint16_t status;
    uint64_t time_us;
    uint16_t stepper_position[NumStepper];
    uint16_t stepper_velocity[NumStepper];
    uint16_t analog_input[NumAnalogInput];
    uint16_t pwm_position[NumPwm];
    uint16_t trigger_prescalar;
    uint8_t trigger_enable;
    uint8_t digital_output;
};

struct __attribute__((packed)) HostToDevMsg 
{
    uint8_t data[64];
};

bool quit_flag = false;
void sig_int_handler(int param);


int main(int argc, char *argv[])
{
    RawHIDDevice hid_dev = RawHIDDevice(Vid,Pid);
    bool rtn_val;

    std::signal(SIGINT,sig_int_handler);

    rtn_val =  hid_dev.open();
    if (!rtn_val)
    {
        std::cerr << "error: unable to open device" << std::endl;
    }
    else
    {
        std::cout << "device opened" << std::endl;
    }

    uint64_t time_us_last = 0;
    while (!quit_flag)
    {
        DevToHostMsg dev_to_host_msg;
        rtn_val = hid_dev.recvData(&dev_to_host_msg);
        if (!rtn_val)
        {
            std::cerr << "error: recvData failure" << std::endl;
            if (quit_flag)
            {
                break;
            }
            continue;
        }
        else
        {
            uint64_t dt = dev_to_host_msg.time_us - time_us_last;
            time_us_last = dev_to_host_msg.time_us;

            std::cout << "time_us = " << dev_to_host_msg.time_us  << " " << dt << std::endl;
            for (int i=0;i<NumAnalogInput; i++)
            {
                std::cout << "ain[" << i << "] = " << dev_to_host_msg.analog_input[i] << std::endl;
            }

            HostToDevMsg host_to_dev_msg;
            rtn_val = hid_dev.sendData(&host_to_dev_msg);
            if (!rtn_val)
            {
                std::cerr << "error: sendData" << std::endl;
            }
        }
    }

    hid_dev.close();
    std::cout << "device closed" << std::endl;

    return 0;
}

void sig_int_handler(int param)
{
    quit_flag = true;
}

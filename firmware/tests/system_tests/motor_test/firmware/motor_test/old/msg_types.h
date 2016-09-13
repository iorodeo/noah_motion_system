#ifndef TYPES_H
#define TYPES_H

#include "constants.h"

struct __attribute__((packed)) MsgDevToHost
{
    uint8_t status;
    uint8_t mode;
    uint64_t time;
    uint16_t pos[constants::NumMotor];
    uint16_t vel[constants::NumMotor];
    uint16_t pwm[constants:NumPwm];
    uint8_t padding[54 - 4*constants::NumMotor - 2*constants::NumPwm];
};

struct __attribute__((packed)) MsgHostToDev
{
    uint16_t cmd;
    uint16_t vel[constants::NumMotor];
    uint16_t pwm[constants:NumPwm];

};

struct __attribute__((packed)) CmdMsg 
{
    uint16_t  msg_id;
    uint16_t  cmd_id; 
    uint8_t   data[60];
};

struct __attribute__((packed)) RspMsg
{
    uint16_t  msg_id;
    uint16_t  cmd_id;
    bool      success;
    char      message[59];
};

struct __attribute__((packed)) PosMsg
{
    uint16_t  msg_id;
    uint64_t  time;
    int32_t   position[constants::NumMotor];
    bool      quit_flag;
    uint8_t   padding[53-constants::NumMotor*4];
};


struct __attribute__((packed)) VelMsg
{
    uint16_t  msg_id;
    float     velocity[constants::NumMotor];
    bool      quit_flag;
    uint8_t   padding[61-constants::NumMotor*4];
};


#endif

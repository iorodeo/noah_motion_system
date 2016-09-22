#ifndef RAWHID_MSG_TYPES_H
#define RAWHID_MSG_TYPES_H

#ifndef __linux__
#include "Arduino.h"
#endif

#include "dev_constants.h"

#ifdef __linux__
#include <cstdint>
namespace mctl {
#endif

struct __attribute__((packed)) DevToHostMsg 
{
    uint64_t time_us;
    int16_t  stepper_position[constants::NumStepper];
    int16_t  stepper_velocity[constants::NumStepper];
    uint16_t pwm_position[constants::NumPwm];
    uint16_t analog_input[constants::NumAnalogInput];
    uint8_t  status;  
    uint8_t  count;
    uint8_t  command;
    uint16_t command_data;
    uint8_t  padding;
};

struct __attribute__((packed)) HostToDevMsg 
{
    uint64_t time_us;
    int16_t  stepper_position[constants::NumStepper];
    int16_t  stepper_velocity[constants::NumStepper];
    uint16_t pwm_position[constants::NumPwm];
    uint16_t trigger_cnt[constants::NumTrigger];
    uint8_t  trigger_enable[constants::NumTrigger]; 
    uint8_t  digital_output[constants::NumDigitalOutput];
    uint8_t  count;
    uint8_t  command;
    uint16_t command_data[8];
};

#ifdef __linux__
}
#endif

#endif


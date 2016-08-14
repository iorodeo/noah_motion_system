#ifndef RAWHID_MSG_TYPES_H
#define RAWHID_MSG_TYPES_H
#include "constants.h"
#include "Arduino.h"

struct __attribute__((packed)) DevToHostMsg 
{
    uint16_t status;
    uint64_t time_us;
    uint16_t stepper_position[constants::NumStepper];
    uint16_t stepper_velocity[constants::NumStepper];
    uint16_t pwm_position[constants::NumPwm];
    uint16_t analog_input[constants::NumAnalogInput];
    uint16_t trigger_prescalar;
    uint8_t trigger_enable;
    uint8_t digital_output;
};

struct __attribute__((packed)) HostToDevMsg 
{
    uint16_t command;
    uint64_t time_us;
    uint16_t stepper_position[constants::NumStepper];
    uint16_t stepper_velocity[constants::NumStepper];
    uint16_t pwm_position[constants::NumPwm];
    uint16_t trigger_prescalar;
    uint8_t trigger_enable;
    uint8_t digital_output;
    uint8_t padding[20];
};


#endif

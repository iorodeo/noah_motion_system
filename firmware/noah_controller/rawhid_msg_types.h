#ifndef RAWHID_MSG_TYPES_H
#define RAWHID_MSG_TYPES_H
#include "constants.h"
#include "Arduino.h"

struct __attribute__((packed)) DevToHostMsg 
{
    uint16_t status;
    uint64_t time_us;
    int16_t stepper_position[constants::NumStepper];
    int16_t stepper_velocity[constants::NumStepper];
    uint16_t pwm_position[constants::NumPwm];
    uint16_t analog_input[constants::NumAnalogInput];
    uint16_t trigger_cnt[constants::NumTrigger];
};

struct __attribute__((packed)) HostToDevMsg 
{
    uint16_t command;
    uint64_t time_us;
    int16_t stepper_position[constants::NumStepper];
    int16_t stepper_velocity[constants::NumStepper];
    uint16_t pwm_position[constants::NumPwm];
    uint16_t trigger_cnt[constants::NumTrigger];
    uint8_t trigger_enable[constants::NumTrigger]; 
    uint8_t digital_output[constants::NumDigitalOutput];
    uint8_t padding[16];
};


#endif

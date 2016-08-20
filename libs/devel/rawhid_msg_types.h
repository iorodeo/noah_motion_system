#ifndef RAWHID_MSG_TYPES_H
#define RAWHID_MSG_TYPES_H
#include <cstdint>

const int NumStepper = 5;
const int NumAnalogInput = 10;
const int NumPwm = 5;
const int NumTrigger = 2;
const int NumDigitalOutput = 2;

struct __attribute__((packed)) DevToHostMsg 
{
    uint64_t time_us;
    int16_t  stepper_position[NumStepper];
    int16_t  stepper_velocity[NumStepper];
    uint16_t pwm_position[NumPwm];
    uint16_t analog_input[NumAnalogInput];
    uint8_t  status;  
    uint8_t  count;
    uint8_t  command;
    uint16_t command_data;
    uint8_t  padding;
};

struct __attribute__((packed)) HostToDevMsg 
{
    uint64_t time_us;
    int16_t  stepper_position[NumStepper];
    int16_t  stepper_velocity[NumStepper];
    uint16_t pwm_position[NumPwm];
    uint16_t trigger_cnt[NumTrigger];
    uint8_t  trigger_enable[NumTrigger]; 
    uint8_t  digital_output[NumDigitalOutput];
    uint8_t  count;
    uint8_t  command;
    uint16_t command_data[8];
};

#endif

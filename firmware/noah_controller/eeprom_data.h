#ifndef EEPROM_DATA_H
#define EEPROM_DATA_H
#include <Arduino.h>
#include "dev_constants.h"

struct EEPROM_Data
{
    bool flag;
    int8_t  stepper_homing_direction[constants::NumStepper];
    int32_t stepper_minimum_position[constants::NumStepper];
    int32_t stepper_maximum_position[constants::NumStepper];
    int32_t stepper_maximum_speed[constants::NumStepper];
    int32_t stepper_maximum_accel[constants::NumStepper];
};

#endif 

#ifndef MOTOR_PIN_H
#define MOTOR_PIN_H
#include <Arduino.h>

struct MotorPin
{
    uint8_t clk;
    uint8_t dir;
};

#endif

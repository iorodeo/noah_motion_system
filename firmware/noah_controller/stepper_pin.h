#ifndef STEPPER_PIN_H
#define STEPPER_PIN_H
#include <Arduino.h>

struct StepperPin
{
    uint8_t clk;
    uint8_t dir;
};

#endif

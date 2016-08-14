#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "stepper_pin.h"

namespace constants
{
    // Operating modes
    enum OperatingMode
    {
        Disabled = 0,
        Idle = 1,
        HomeAxis = 2,
        MoveToPosition = 3,
        VelocityControl = 4
    };

    // Communications timeouts
    extern const uint16_t DevToHostTimeout;
    extern const uint16_t HostToDevTimeout;

    // Timing parameters 
    extern const uint8_t  TimerPriority;
    extern const uint32_t TimerPeriod;
    extern const uint32_t MessagePeriod;
    extern const uint32_t NewMessageCount;

    // Analog input parameters
    const uint8_t NumAnalogInput = 10;
    extern const uint8_t AnalogReadNumBits;
    extern const uint8_t AnalogReadNumAvg;
    extern const uint8_t AnalogRefType;
    extern const uint8_t AnalogInputPinArray[NumAnalogInput]; 

    // PWM parameters
    const uint8_t NumPwm = 5;
    extern const float PwmFrequency;
    extern const uint8_t PwmResolution;
    extern const uint16_t PwmMinValue;
    extern const uint16_t PwmMaxValue;
    extern const uint16_t PwmDefaultValue;
    extern const uint8_t PwmPinArray[NumPwm];

    // Stepper parameters
    const uint8_t NumStepper = 5;
    extern const int16_t StepperMaximumSpeed[NumStepper];
    extern const int16_t StepperMinimumPosition[NumStepper];
    extern const int16_t StepperMaximumPosition[NumStepper];
    extern const int16_t StepperHomePosition[NumStepper];
    extern const StepperPin StepperPinArray[NumStepper];

    // Trigger parameters
    const uint8_t  NumTrigger = 2;
    extern const uint8_t TriggerPinArray[NumTrigger];

    // Digital output parameters
    const uint8_t NumDigitalOutput = 2;
    extern const uint8_t  DigitalOutputPinArray[NumDigitalOutput];

    // Default boundary parameters

    // Homing parameters

    // Positioning parameters

    // Velocity control parameters

}
#endif

#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "stepper_pin.h"

namespace constants
{
    // Communications timeouts
    extern const int DevToHostTimeout;
    extern const int HostToDevTimeout;

    // Timing parameters 
    extern const unsigned int  TimerPriority;
    extern const unsigned long TimerPeriod;
    extern const unsigned long MessagePeriod;
    extern const unsigned long NewMessageCount;

    // Analog input parameters
    extern const unsigned int AnalogReadNumBits;
    extern const unsigned int AnalogReadNumAvg;
    extern const int AnalogRefType;

    // PWM parameters
    extern const int PwmResolution;
    extern const float PwmFrequency;
    extern const int PwmMinValue;
    extern const int PwmMaxValue;
    extern const int PwmDefaultValue;

    // Number of pin types
    const int NumStepper = 5;
    const int NumAnalogInput = 10;
    const int NumPwm = 5;
    const int NumTrigger = 2;
    const int NumDigitalOutput = 2;

    // Pin mappings
    extern const StepperPin StepperPinArray[NumStepper];
    extern const int PwmPinArray[NumPwm];
    extern const int AnalogInputPinArray[NumAnalogInput]; 
    extern const int TriggerPinArray[NumTrigger];
    extern const int DigitalOutputPin[NumDigitalOutput];
}
#endif

#include "constants.h"
#include "Arduino.h"

namespace constants
{
    // Communications timeouts
    const int DevToHostTimeout = 50;
    const int HostToDevTimeout = 50;

    // Timing parameters 
    extern const unsigned int  TimerPriority = 1;
    const unsigned long TimerPeriod = 50ul;
    const unsigned long MessagePeriod = 5000ul;
    const unsigned long NewMessageCount = MessagePeriod/TimerPeriod;

    // Analog input parameters
    extern const unsigned int AnalogReadNumBits = 16;
    extern const unsigned int AnalogReadNumAvg = 32;
    extern const int AnalogRefType = INTERNAL;

    // PWM parameters
    const int PwmResolution = 16;
    const float PwmFrequency = 50.0;
    const int PwmMinValue = 3276;
    const int PwmMaxValue = 6553;
    const int PwmDefaultValue = (PwmMinValue + PwmMaxValue)/2;
    
    // Pin mappings
    const StepperPin StepperPinArray[NumStepper] = {{2,3},{4,5},{6,7},{8,9},{24,25}};
    const int PwmPinArray[NumPwm] = {20,21,22,23,10};
    const int AnalogInputPinArray[NumAnalogInput] = {A0,A1,A2,A3,A4,A5,A10,A11,A12,A13};
}

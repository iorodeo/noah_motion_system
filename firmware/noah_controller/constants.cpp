#include "constants.h"
#include "Arduino.h"

namespace constants
{
    // Communications timeouts
    const uint16_t DevToHostTimeout = 50;
    const uint16_t HostToDevTimeout = 50;

    // Timing parameters 
    const uint8_t  TimerPriority = 1;
    const uint32_t TimerPeriod = 50ul;
    const uint32_t MessagePeriod = 5000ul;
    const uint32_t NewMessageCount = MessagePeriod/TimerPeriod;

    // Analog input parameters
    extern const uint8_t AnalogReadNumBits = 16;
    extern const uint8_t AnalogReadNumAvg = 32;
    extern const uint8_t AnalogRefType = INTERNAL;
    const uint8_t AnalogInputPinArray[NumAnalogInput] = {A0,A1,A2,A3,A4,A5,A10,A11,A12,A13};

    // PWM parameters
    const float PwmFrequency = 50.0;
    const uint8_t PwmResolution = 16;
    const uint16_t PwmMinValue = 3276;
    const uint16_t PwmMaxValue = 6553;
    const uint16_t  PwmDefaultValue = (PwmMinValue + PwmMaxValue)/2;
    const uint8_t PwmPinArray[NumPwm] = {20,21,22,23,10};

    // Stepper parameters
    const int16_t StepperMaximumSpeed[NumStepper]    = {  8000,   8000,   8000,   8000,   8000    };
    const int16_t StepperMinimumPosition[NumStepper] = { -10000, -10000, -10000, -10000, -10000   };
    const int16_t StepperMaximumPosition[NumStepper] = {  10000,  10000,  10000,  10000,  10000   };
    const int16_t StepperHomePosition[NumStepper]    = { -10000, -10000, -10000, -10000, -10000   };
    const StepperPin StepperPinArray[NumStepper]     = {  {2,3},  {4,5},  {6,7},  {8,9},  {24,25} };
    
    // Trigger parameters 
    const uint8_t TriggerPinArray[NumTrigger] = {26,27};

    // Digital output parameters
    const uint8_t DigitalOutputPinArray[NumDigitalOutput] = {0,1};
}

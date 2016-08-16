#include "constants.h"
#include "Arduino.h"

namespace constants
{
    uint8_t NumModeBits = 4; 

    // Communications timeouts
    const uint16_t DevToHostTimeout = 50;
    const uint16_t HostToDevTimeout = 50;

    // Timing parameters 
    const uint8_t  TimerPriority = 1;
    const uint32_t TimerPeriod = 50ul;
    const uint32_t MessagePeriod = 5000ul;
    const uint32_t NewMessageCount = MessagePeriod/TimerPeriod;

    // Analog input parameters
    const uint8_t AnalogReadNumBits = 16;
    const uint8_t AnalogReadNumAvg = 32;
    const uint8_t AnalogRefType = INTERNAL;
    const uint8_t AnalogInputPinArray[NumAnalogInput] = {A0,A1,A2,A3,A4,A5,A10,A11,A12,A13};

    // PWM parameters
    const float PwmFrequency = 50.0;
    const uint8_t PwmResolution = 16;
    const uint16_t PwmMinimumValue = 3276;
    const uint16_t PwmMaximumValue = 6553;
    const uint16_t  PwmDefaultValue = (PwmMinimumValue + PwmMaximumValue)/2;
    const uint8_t PwmPinArray[NumPwm] = {20,21,22,23,10};

    // Stepper parameters - all units in steps, steps/sec and steps/sec**2
    const int16_t StepperMaximumSpeed[NumStepper]    = {  8000,   8000,   8000,   8000,   8000    };
    const int16_t StepperMaximumAccel[NumStepper]    = {  10000,  10000,  10000,  10000,  10000   }; 
    const int16_t StepperMinimumPosition[NumStepper] = { -10000, -10000, -10000, -10000, -10000   };
    const int16_t StepperMaximumPosition[NumStepper] = {  10000,  10000,  10000,  10000,  10000   };  
    const int16_t StepperHomePosition[NumStepper]    = { -10000, -10000, -10000, -10000, -10000   };
    const StepperPin StepperPinArray[NumStepper]     = {  {2,3},  {4,5},  {6,7},  {8,9},  {24,25} };
    const uint8_t StepperClockHighDelay = 1;  // us

    // Homing parameters

    // Move to position parameters

    // Velocity control parameters
    
    // Trigger parameters 
    const bool DefaultTriggerEnabled[NumTrigger] = {false,false};
    const uint16_t DefaultTriggerCount[NumTrigger] = {10000/TimerPeriod, 10000/TimerPeriod};
    const uint8_t TriggerPinArray[NumTrigger] = {26,27};
    const uint8_t  TriggerHighDelay = 1; // us


    // Digital output parameters
    const uint8_t DigitalOutputPinArray[NumDigitalOutput] = {0,1};
    const uint8_t DefaultDigitalOutputValue[NumDigitalOutput] = {LOW, LOW};
}

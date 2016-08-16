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
    extern uint8_t NumModeBits; 

    // Commands
    enum UsbCommand
    {
        EmptyCmd   = 0,

        SetModeToDisabled,
        SetModeToEnabled,
        SetModeToIdle,
        SetModeToMoveToPosition,
        SetModetoHomeAxis,
        SetModeToVelocityControl,

        GetTriggerCount,
        SetTriggerCount,
        GetTriggerEnabled,
        GetDigitalOutput

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
    extern const uint16_t PwmMinimumValue;
    extern const uint16_t PwmMaximumValue;
    extern const uint16_t PwmDefaultValue;
    extern const uint8_t PwmPinArray[NumPwm];

    // Stepper parameters - all units in steps, steps/sec and steps/sec**2
    const uint8_t NumStepper = 5;
    extern const int16_t StepperMaximumSpeed[NumStepper];
    extern const int16_t StepperMaximumAccel[NumStepper];
    extern const int16_t StepperMinimumPosition[NumStepper];
    extern const int16_t StepperMaximumPosition[NumStepper];
    extern const int16_t StepperHomePosition[NumStepper];
    extern const StepperPin StepperPinArray[NumStepper];
    extern const uint8_t StepperClockHighDelay;

    // Trigger parameters
    const uint8_t  NumTrigger = 2;
    extern const bool DefaultTriggerEnabled[NumTrigger];
    extern const uint16_t DefaultTriggerCount[NumTrigger];
    extern const uint8_t TriggerPinArray[NumTrigger];
    extern const uint8_t TriggerHighDelay;

    // Digital output parameters
    const uint8_t NumDigitalOutput = 2;
    extern const uint8_t DigitalOutputPinArray[NumDigitalOutput];
    extern const uint8_t DefaultDigitalOutputValue[NumDigitalOutput];

    // Homing parameters

    // Positioning parameters

    // Velocity control parameters
    
    // Status information bits

}
#endif

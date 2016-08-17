#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "stepper_pin.h"

namespace constants
{
    // Operating modes
    enum OperatingMode
    {
        Mode_Disabled = 0,
        Mode_Enabled,
        Mode_HomeAxis,
        Mode_MoveToPosition,
        Mode_VelocityControl
    };
    extern uint8_t NumModeBits; 

    // Commands
    enum UsbCommand
    {
        Cmd_Empty   = 0,
        Cmd_SetMode_Disabled,
        Cmd_SetMode_Enabled,
        Cmd_SetMode_MoveToPosition,
        Cmd_SetMode_HomeAxis,
        Cmd_SetMode_VelocityControl,
        Cmd_Get_TriggerCount,
        Cmd_Set_TriggerCount,
        Cmd_Get_TriggerEnabled,
        Cmd_Get_DigitalOutput
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
    extern const uint8_t StepperDriveEnablePin;

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


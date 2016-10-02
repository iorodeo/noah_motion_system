#ifndef DEV_CONSTANTS_H
#define DEV_CONSTANTS_H

#ifndef __linux__
#include "Arduino.h"
#include "stepper_pin.h"
#else
#include <cstdint>
#endif

namespace constants
{
    // Operating modes
    enum OperatingMode
    {
        Mode_Disabled = 0,
        Mode_Ready,
        Mode_Homing,
        Mode_Positioning,
        Mode_VelocityControl
    };
    extern const uint8_t NumModeBits; 
    extern const uint8_t ModeBitsMask;

    // Commands
    enum UsbCommand
    {
        Cmd_Empty   = 0,
        Cmd_SetModeDisabled,
        Cmd_SetModeReady,
        Cmd_SetModeHoming,
        Cmd_SetModePositioning,
        Cmd_SetModeVelocityControl,
        Cmd_StopMotion,
        Cmd_SetAxisHomed,
        Cmd_GetAxisHomed,
        Cmd_GetTriggerCount,
        Cmd_SetTriggerCount,
        Cmd_SetTriggerEnabled,
        Cmd_GetTriggerEnabled,
        Cmd_SetStepperPosition,
        Cmd_GetDigitalOutput
    };

    const uint8_t NumAnalogInput = 10;
    const uint8_t NumPwm = 5;
    const uint8_t NumStepper = 5;
    const uint8_t NumTrigger = 2;
    const uint8_t NumDigitalOutput = 2;

    // Timing parameters - shared with host software
    extern const uint32_t TimerPeriod_us;
    extern const uint32_t MessagePeriod_us;

    // Stepper parameters - shared with host software
    // all units in steps, steps/sec and steps/sec**2
    extern const int32_t StepperMaximumSpeed[NumStepper];
    extern const int32_t StepperMaximumAccel[NumStepper];
    extern const int32_t StepperMinimumPosition[NumStepper];
    extern const int32_t StepperMaximumPosition[NumStepper];
    extern const int32_t StepperHomePosition[NumStepper];

    extern const float PwmFrequency;
    extern const uint8_t PwmResolution;
    extern const uint16_t PwmMinimumValue;
    extern const uint16_t PwmMaximumValue;
    extern const uint16_t PwmDefaultValue;

    extern const int8_t HomingDirection[NumStepper];

#ifndef __linux__

    // Communications timeouts
    extern const uint16_t DevToHostTimeout;
    extern const uint16_t HostToDevTimeout;

    // Timing parameters 
    extern const uint8_t  TimerPriority;
    extern const uint32_t NewMessageCount;

    // Analog input parameters
    extern const uint8_t AnalogReadNumBits;
    extern const uint8_t AnalogReadNumAvg;
    extern const uint8_t AnalogRefType;
    extern const uint8_t AnalogInputPinArray[NumAnalogInput]; 

    // PWM parameters - firmware only
    extern const uint8_t PwmPinArray[NumPwm];

    // Stepper parameters - firmware only
    extern const StepperPin StepperPinArray[NumStepper];
    extern const uint8_t StepperClockHighDelay;
    extern const uint8_t StepperDriveEnablePin;

    // PositionController  parameters
    extern const uint32_t PositionControllerMaximumSpeed[NumStepper];
    extern const uint32_t PositionControllerGain[NumStepper];

    // Homing parameters
    extern const uint8_t HomingInterruptPin;
    extern const uint32_t HomingSpeed[NumStepper]; 

    // Trigger parameters
    extern const bool DefaultTriggerEnabled[NumTrigger];
    extern const uint16_t DefaultTriggerCount[NumTrigger];
    extern const uint8_t TriggerPinArray[NumTrigger];
    extern const uint8_t TriggerHighDelay;

    // Digital output parameters
    extern const uint8_t DigitalOutputPinArray[NumDigitalOutput];
    extern const uint8_t DefaultDigitalOutputValue[NumDigitalOutput];

    // E-Stop Monitor parameters 
    extern const uint8_t EStopMonitorPin;
    extern const uint16_t EStopMonitorThreshold;
    extern const uint32_t EStopMonitorStartupDelay;
#endif 
    

}
#endif


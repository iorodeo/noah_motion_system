#include "dev_constants.h"

namespace constants
{
    const uint8_t NumModeBits = 4; 
    const uint8_t ModeBitsMask = 0x0f;

    // Timing parameters - shared with host software
    const uint32_t TimerPeriod_us = 50ul;
    const uint32_t MessagePeriod_us = 5000ul;

    // PWM parameter - shared with host software
    const float PwmFrequency = 50.0;
    const uint8_t PwmResolution = 16;
    const uint16_t PwmMinimumValue = 3276;
    const uint16_t PwmMaximumValue = 6553;
    const uint16_t  PwmDefaultValue = (PwmMinimumValue + PwmMaximumValue)/2;

    // Stepper parameters  - shared with host software
    // all units in steps, steps/sec and steps/sec**2
    const int32_t DefaultStepperMaximumSpeed[NumStepper]    = {  8000,   8000,   8000,   8000,   8000    };
    const int32_t DefaultStepperMaximumAccel[NumStepper]    = {  10000,  10000,  10000,  10000,  10000   }; 
    const int32_t DefaultStepperMinimumPosition[NumStepper] = { -10000, -10000, -10000, -30000, -30000   };
    const int32_t DefaultStepperMaximumPosition[NumStepper] = {  10000,  10000,  10000,  30000,  30000   };  

    // Homing parameters - shared with host software
    const int8_t DefaultStepperHomingDirection[NumStepper] = {1,1,1,1,1}; // 1 or -1.

#ifndef  __linux__

    // Communications timeouts
    const uint16_t DevToHostTimeout = 50;
    const uint16_t HostToDevTimeout = 50;

    // EEPROM parameters
    const uint16_t DefaultConfigEEPROMAddress = 0;

    // Timing parameters 
    const uint8_t  TimerPriority = 1;
    const uint32_t NewMessageCount = MessagePeriod_us/TimerPeriod_us;

    // Analog input parameters
    const uint8_t AnalogReadNumBits = 16;
    const uint8_t AnalogReadNumAvg = 32;
    const uint8_t AnalogRefType = INTERNAL;
    const uint8_t AnalogInputPinArray[NumAnalogInput] = {A0,A1,A2,A3,A4,A5,A10,A11,A12,A13};

    // PWM parameters - firmware only
    const uint8_t PwmPinArray[NumPwm] = {20,21,22,23,10};

    // Stepper parameters - all units in steps, steps/sec and steps/sec**2
    const StepperPin StepperPinArray[NumStepper] = {{2,3}, {4,5}, {6,7}, {8,9}, {24,25}};
    const uint8_t StepperClockHighDelay = 1;  // us
    const uint8_t StepperDriveEnablePin = 32;

    // PositionController  parameters
    const uint32_t PositionControllerMaximumSpeed[NumStepper] = {1000, 1000, 3000, 1000, 1000};
    const uint32_t PositionControllerGain[NumStepper] = {3, 3, 3, 3, 3};

    // Homing parameters
    const uint8_t HomingInterruptPin = 28;
    const uint32_t HomingSpeed[NumStepper] = {200,200,200,2000,200};

    // Trigger parameters 
    const bool DefaultTriggerEnabled[NumTrigger] = {true,false};
    const uint16_t DefaultTriggerCount[NumTrigger] = {10000/TimerPeriod_us, 10000/TimerPeriod_us};
    const uint8_t TriggerPinArray[NumTrigger] = {26,27};
    const uint8_t  TriggerHighDelay = 1; // us

    // Digital output parameters
    const uint8_t DigitalOutputPinArray[NumDigitalOutput] = {0,1};
    const uint8_t DefaultDigitalOutputValue[NumDigitalOutput] = {LOW, LOW};

    // E-Stop Monitor parameters 
    const uint8_t EStopMonitorPin = A20;
    const uint16_t EStopMonitorThreshold = 40000;
    const uint32_t EStopMonitorStartupDelay = 200;
#endif

};


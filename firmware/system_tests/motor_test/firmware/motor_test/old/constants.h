#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <Arduino.h>
#include "motor_pin.h"

namespace constants
{
    const int NumMotor = 5;
    const int NumPwm = 4;
    extern const int MsgSize;
    extern const int CmdModeMsgTimeout;
    extern const int VelModeMsgTimeout;
    extern const unsigned long TimerPeriod;
    extern const unsigned long VeloUpdateCnt;
    extern const MotorPin MotorPinArray[NumMotor];
    extern const float MaxMotorSpeed;

} // namespace constants 

#endif

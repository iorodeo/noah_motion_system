#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <Arduino.h>
#include "motor_pin.h"

namespace constants
{
    const int NumMotor = 5;

    enum MsgId
    {
        ErrMsgId       = 0,
        CmdMsgId       = 1,
        VelMsgId       = 2,
        PosMsgId       = 3,
        MsgIdForceSize = 0xffff
    };
    
    enum CmdId
    {
        CmdError         = 0,
        CmdStartVelCtrl  = 1,
        CmdSetMotorPos   = 2,
        CmdSetMotorVel   = 3,
        CmdIdForceSize   = 0xffff
    };
    
    enum OperatingModeId
    {
        OpModeCmd = 1,
        OpModeVel = 2
    };


    extern const int DriveEnablePin;
    extern const int MsgSize;
    extern const int CmdModeMsgTimeout;
    extern const int VelModeMsgTimeout;
    extern const unsigned long TimerPeriod;
    extern const unsigned long VeloUpdateCnt;
    extern const MotorPin MotorPinArray[NumMotor];
    extern const float MaxMotorSpeed;


} // namespace constants 

#endif

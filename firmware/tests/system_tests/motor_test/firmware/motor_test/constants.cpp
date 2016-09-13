#include "constants.h"

namespace constants
{
    const int DriveEnablePin = 32;
    const int MsgSize = 64;
    const int CmdModeMsgTimeout = 50;
    const int VelModeMsgTimeout = 50;
    const unsigned long TimerPeriod = 50ul;
    const unsigned long VeloUpdateCnt = 5000ul/TimerPeriod;
    const MotorPin MotorPinArray[NumMotor] = {{2,3},{4,5},{6,7},{8,9},{24,25}};
    const float MaxMotorSpeed = 1.0/(TimerPeriod*1.0e-6);

} // namespace constants

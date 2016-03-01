#include "constants.h"

namespace constants
{

    const int MsgSize = 64;
    const int CmdModeMsgTimeout = 50;
    const int VelModeMsgTimeout = 50;
    const unsigned long TimerPeriod = 5000;

    const MotorPin MotorPinArray[NumMotor] = {{2,3},{4,5},{6,7},{8,9},{10,11},{12,13}};
    const float MaxMotorSpeed = 50000.0;

} // namespace constants

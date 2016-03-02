#include "constants.h"

namespace constants
{

    const int MsgSize = 64;
    const int CmdModeMsgTimeout = 50;
    const int VelModeMsgTimeout = 50;
    //const unsigned long TimerPeriod = 5000;
    const unsigned long TimerPeriod = 20;

    //const MotorPin MotorPinArray[NumMotor] = {{14,15},{16,17},{18,19},{20,21},{22,23},{24,25}};
    const MotorPin MotorPinArray[NumMotor] = {{14,15},{16,17}};//,{18,19},{20,21},{22,23},{24,25}};
    const float MaxMotorSpeed = 50000.0;

} // namespace constants

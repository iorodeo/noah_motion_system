#include "constants.h"

namespace constants
{

    const int MsgSize = 64;
    const int CmdModeMsgTimeout = 50;
    const int VelModeMsgTimeout = 50;
    const unsigned long TimerPeriod = 5000;

    const MotorPin MotorPinArray[NumMotor] = {{6,7},{8,9},{10,11},{12,13},{14,15},{16,17}};
    const float MaxMotorSpeed = 50000.0;

    const int DisplayDC = 5;
    const int DisplayCS = 3;
    const int DisplayReset = 4;
    const SPISettings DisplaySPISettings(4000000,MSBFIRST,SPI_MODE0);

} // namespace constants

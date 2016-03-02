#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace constants
{
    const int NumMotor = 2;

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

    extern const int Vid; 
    extern const int Pid;
    extern const int RawHidTimeout;

    extern const double PGain;
    extern const double FGain;
    extern const double Dt;
    extern const double JoyToForce;

} // namespace constants 

#endif

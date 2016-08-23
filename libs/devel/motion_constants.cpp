#include "motion_constants.hpp"

namespace motion
{

    const int USB_VendorId = 0x16C0;
    const int USB_ProductId = 0x0486;

    const int HomingDebounceSleep_ms = 500;

    const std::list<Axis> AxisList = 
    {
        Axis_X, 
        Axis_Y, 
        Axis_Z, 
        Axis_A, 
        Axis_B, 
        Pwm_0, 
        Pwm_1, 
        Pwm_2, 
        Pwm_3, 
        Pwm_4
    };

    const std::list<Axis> StepperList = 
    {
        Axis_X, 
        Axis_Y, 
        Axis_Z, 
        Axis_A, 
        Axis_B 
    }; 

    const std::list<Axis> PwmList = 
    {
        Pwm_0, 
        Pwm_1, 
        Pwm_2, 
        Pwm_3, 
        Pwm_4 
    };


    std::map<OperatingMode,std::string> OperatingModeToStringMap = 
    {
        {Mode_Disabled,         std::string("Disabled")},
        {Mode_Ready,            std::string("Ready")},
        {Mode_Homing,           std::string("Homing")},
        {Mode_Positioning,      std::string("Positioning")},
        {Mode_VelocityControl,  std::string("VelocityControl")}
    };


    std::map<Axis,std::string> AxisToStringMap = 
    {
        {Axis_X, std::string("Axis_X")},
        {Axis_Y, std::string("Axis_Y")},
        {Axis_Z, std::string("Axis_Z")},
        {Axis_A, std::string("Axis_A")},
        {Axis_B, std::string("Axis_B")},
        {Pwm_0,  std::string("Pwm_0")},
        {Pwm_1,  std::string("Pwm_1")},
        {Pwm_2,  std::string("Pwm_2")},
        {Pwm_3,  std::string("Pwm_3")},
        {Pwm_4,  std::string("Pwm_4")},
    };


    std::map<Unit,std::string> UnitToStringMap =
    {
        {Meter,  std::string("m")},
        {Degree, std::string("deg")},
    };


    std::map<Axis,Unit> DefaultAxisToUnitMap = 
    { 
        {Axis_X, Meter},
        {Axis_Y, Meter},
        {Axis_Z, Meter},
        {Axis_A, Degree},
        {Axis_B, Degree},
        {Pwm_0,  Degree},
        {Pwm_1,  Degree},
        {Pwm_2,  Degree},
        {Pwm_3,  Degree},
    };


    std::map<Axis,double> DefaultAxisToUnitConversionMap =
    {
        {Axis_X, 3.989832e-05}, // CRP NEMA 34 standard rack and pinon drive
        {Axis_Y, 3.989832e-05}, // CRP NEMA 34 standard rack and pinon drive
        {Axis_Z, 1.270000e-5},  // CRP standard ACME lead screw drive 
        {Axis_A, 1.270000e-5},  // CRP standard ACME lead screw drive 
        {Axis_B, 1.270000e-5},  // CRP standard ACME lead screw drive 
        {Pwm_0,  0.0344853},    // Generic RC servo 113 deg/ms with 16-bit pwm
        {Pwm_1,  0.0344853},    // Generic RC servo 113 deg/ms with 16-bit pwm
        {Pwm_2,  0.0344853},    // Generic RC servo 113 deg/ms with 16-bit pwm
        {Pwm_3,  0.0344853},    // Generic RC servo 113 deg/ms with 16-bit pwm
    };


    std::map<Axis, bool> DefaultHomingEnabledMap = 
    {
        {Axis_X, false},
        {Axis_Y, false},
        {Axis_Z, false},
        {Axis_A, false},
        {Axis_B, false},
        {Pwm_0,  false},
        {Pwm_1,  false},
        {Pwm_2,  false},
        {Pwm_3,  false},
    };


}

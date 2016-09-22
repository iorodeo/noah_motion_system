#include "constants.hpp"

namespace mctl
{

    const int USB_VendorId = 0x16C0;
    const int USB_ProductId = 0x0486;


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
        {Mode_Disabled,         std::string("disabled")},
        {Mode_Ready,            std::string("ready")},
        {Mode_Homing,           std::string("homing")},
        {Mode_Positioning,      std::string("positioning")},
        {Mode_VelocityControl,  std::string("velocity control")}
    };


    std::map<Axis,std::string> AxisToStringMap = 
    {
        {Axis_X, std::string("X")},
        {Axis_Y, std::string("Y")},
        {Axis_Z, std::string("Z")},
        {Axis_A, std::string("A")},
        {Axis_B, std::string("B")},
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
        {Volt,   std::string("V")},
    };

    const int ForceAndTorqueRowDim = 6;
    const int HomingDebounceSleep_ms = 500;
    const int DefaultOutscanStartDelay_ms = 2000;
    const int32_t DefaultGain = 100;

    const double DefaultAnalogInputScale = (49900.0/3000.0)*(1.2/65535.0); // (Rin/Rf)*(AINREF/MAX_UINT16)
    const double DefaultAnalogInputOffset = -10.0;                         // (-VREF)
    const Unit DefaultAnalogInputUnit = Volt;

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
        {Axis_Z, 1.270000e-05}, // CRP standard ACME lead screw drive 
        {Axis_A, 0.005},        // Sherline rotary axis 
        {Axis_B, 0.005},        // Sherline rotary axis 
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

    std::map<Axis,double> DefaultHomingBackoffMap =
    {
        {Axis_X, 0.005},
        {Axis_Y, 0.005},
        {Axis_Z, 0.005},
        {Axis_A, 5.0},
        {Axis_B, 5.0},
    };

    // Files and Directories
    extern const std::string DefaultConfigurationDir = std::string(".mctl");
    const std::string DefaultConfigurationFile = std::string("config.json");

}

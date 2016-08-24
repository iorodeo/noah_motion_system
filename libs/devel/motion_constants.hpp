#ifndef MOTION_CONSTANTS_HPP
#define MOTION_CONSTANTS_HPP
#include <cstdint>
#include <list>
#include <map>

#include "constants.h"


namespace motion
{
    using namespace constants;

    // Motion Axes
    enum Axis 
    { 
        Axis_X=0, 
        Axis_Y, 
        Axis_Z, 
        Axis_A, 
        Axis_B, 
        Pwm_0, 
        Pwm_1, 
        Pwm_2, 
        Pwm_3, 
        Pwm_4, 
        NumAxis
    };

    enum Unit {Meter=0, Degree, NoUnit, NumUnit};

    // Homing parameters

    // Device USB vendor and product IDs
    extern const int USB_VendorId;
    extern const int USB_ProductId;

    // Axis, Stepper and PWM lists
    extern const std::list<Axis> AxisList;
    extern const std::list<Axis> StepperList;
    extern const std::list<Axis> PwmList;

    // String conversions
    extern std::map<OperatingMode,std::string> OperatingModeToStringMap;
    extern std::map<Unit,std::string> UnitToStringMap;
    extern std::map<Axis,std::string> AxisToStringMap;

    // Unit default assignments and conversions
    extern const int HomingDebounceSleep_ms;
    extern std::map<Axis,Unit> DefaultAxisToUnitMap;
    extern std::map<Axis,double> DefaultAxisToUnitConversionMap;
    extern std::map<Axis, bool> DefaultHomingEnabledMap;
    extern std::map<Axis,double> DefaultHomingBackoffMap;

}

#endif

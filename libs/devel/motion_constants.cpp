#include "motion_constants.hpp"

namespace motion
{
    const int USB_VendorId = 0x16C0;
    const int USB_ProductId = 0x0486;

    const std::list<Axis> AxisList = {Axis_X, Axis_Y, Axis_Z, Axis_A, Axis_B, Pwm_0, Pwm_1, Pwm_2, Pwm_3, Pwm_4};
    const std::list<Axis> StepperList = {Axis_X, Axis_Y, Axis_Z, Axis_A, Axis_B }; 
    const std::list<Axis> PwmList = {Pwm_0, Pwm_1, Pwm_2, Pwm_3, Pwm_4 };

    std::map<OperatingMode,std::string> OperatingModeToStringMap = 
    {
        {Mode_Disabled, std::string("Disabled")},
        {Mode_Ready, std::string("Ready")},
        {Mode_Homing, std::string("Homing")},
        {Mode_Positioning, std::string("Positioning")},
        {Mode_VelocityControl, std::string("VelocityControl")}
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

    const int HomingDebounceSleep_ms = 500;

}

#ifndef MOTION_CONTROLLER_H
#define MOTION_CONTROLLER_H
#include "rawhid_device.hpp"
#include <map>
#include <list>
#include <vector>
#include <functional>
#include <iostream>

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
    NumAxes
};

// Operating modes
enum OperatingMode
{
    Mode_Disabled = 0,
    Mode_Ready,
    Mode_Homing,
    Mode_Positioning,
    Mode_VelocityControl
};
extern const uint8_t NumModeBits; 

// Commands
enum UsbCommand
{
    Cmd_Empty   = 0,
    Cmd_SetModeDisabled,
    Cmd_SetModeReady,
    Cmd_SetModeHoming,
    Cmd_SetModePositioning,
    Cmd_SetModeVelocityControl,
    Cmd_StopMotion,
    Cmd_SetHomePosition,
    Cmd_GetTriggerCount,
    Cmd_SetTriggerCount,
    Cmd_GetTriggerEnabled,
    Cmd_GetDigitalOutput
};


class MotionController
{
    public:

        static const std::list<Axis> AxisList;
        static const std::list<Axis> StepperList;
        static const std::list<Axis> PwmList;

        MotionController(int vid=0, int pid=0);

        bool open();
        void close();
        void test();

        // Homing functions
        // ---------------------------------------------------------
        void enable_homing(Axis axis);
        void disable_homing(Axis axis);
        bool is_homing_enabled(Axis axis);
        void home(Axis axis);

        void move_to_position(Axis axis, int pos);
        void move_to_position(std::vector<int> pos_vec);
        void move_to_position(std::map<Axis,int> pos_map);

        void outscan_trajectory();

        // ----------------------------------------------------------

    private:

        RawHIDDevice hid_dev_;
        std::map<Axis,bool> homing_enabled_map_;

        void set_mode_to_home(int axis);

};

// Using class method as callback
// --------------------------------------------------------------------------------------
//void run(std::function<void(int)> update);
//
// Calling class method
// --------------------------------------------------------------------------------------
//std::function<void(int)> func = std::bind(&Test::update, test, std::placeholders::_1);
//controller.run(func);
#endif

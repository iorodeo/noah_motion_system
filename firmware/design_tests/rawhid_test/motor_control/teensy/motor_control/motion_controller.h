#ifndef MOTION_CONTROLLER_H
#define MOTION_CONTROLLER_H
#include "constants.h"
#include "stepper.h"
//#include <AccelStepper.h>


class MotionController
{
    public:

        MotionController();
        void initialize();
        void update();
        void update_display();
        inline void stepper_update();

        volatile bool velo_update_flag_;

    protected:

        constants::OperatingModeId operating_mode_; 
        //AccelStepper motor_[constants::NumMotor];
        Stepper motor_[constants::NumMotor];

        IntervalTimer velo_timer_;
        bool velo_quit_flag_;

        uint64_t time_us_;
        uint32_t micros_last_; 

        void cmd_mode_update();
        void vel_mode_update();

        //void velo_timer_callback();
};

inline void MotionController::stepper_update()
{
    for (int i=0; i<constants::NumMotor; i++)
    {
        motor_[i].update();
    }
}

#endif

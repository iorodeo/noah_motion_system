#ifndef MOTION_CONTROLLER_H
#define MOTION_CONTROLLER_H
#include "Arduino.h"

class MotionController
{
    public:
        MotionController();
        void initialize();
        void update();

        volatile bool new_msg_flag_;

    private:

        uint16_t mode_;

        IntervalTimer timer_;
        uint64_t time_us_ = 0;
        uint32_t micros_last_ = 0;

};

extern MotionController motion_controller;

#endif 
#ifndef HOMING_CONTROLLER_H
#define HOMING_CONTROLLER_H
#include <Arduino.h>
#include <functional>
#include "velocity_controller.h"

class HomingController
{
    public:

        static void initialize(uint8_t pin);
        static void enable();
        static void disable();
        static void set_home_found(bool value);
        static bool home_found();

        HomingController(int8_t direction=1, uint32_t speed=100);
        void update(int32_t position);
        void reset();

        int32_t velocity();

        uint32_t max_speed();
        void set_max_speed(uint32_t value);

        uint32_t accel();
        void set_accel(uint32_t value);

    protected:

        static bool home_found_;
        static uint8_t pin_;

        VelocityController velocity_controller_;

};

#endif

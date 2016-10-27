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
        static bool pin_state();

        HomingController(int8_t direction=1, uint32_t speed=100);
        int32_t update(int32_t position);
        void reset();

        int32_t velocity();

        uint32_t speed();
        void set_speed(uint32_t speed);

        int8_t direction();
        void set_direction(int8_t direction);

        uint32_t max_speed();
        void set_max_speed(uint32_t max_speed);

        uint32_t accel();
        void set_accel(uint32_t accel);

        void set_velocity_setp(int32_t velocity);
        int32_t velocity_setp();

    protected:

        static bool home_found_;
        static uint8_t pin_;

        uint32_t speed_;
        int8_t direction_;

        VelocityController velocity_controller_;

};

#endif

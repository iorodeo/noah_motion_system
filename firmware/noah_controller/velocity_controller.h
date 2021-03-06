#ifndef VELOCITY_CONTROLLER
#define VELOCITY_CONTROLLER
#include <Arduino.h>

class VelocityController
{
    public: 

        VelocityController(); 

        void reset();

        int32_t velocity();
        void set_velocity(int32_t vel);

        int32_t velocity_setp();
        void set_velocity_setp(int32_t vel);

        int32_t min_position();
        void set_min_position(int32_t pos);

        int32_t max_position();
        void set_max_position(int32_t pos);

        uint32_t max_speed();
        void set_max_speed(uint32_t speed);

        uint32_t max_accel();
        void set_max_accel(uint32_t accel);

        int32_t update(int32_t position);

        void enable_bounds_check();
        void disable_bounds_check();

    protected:

        bool is_first_ = true;
        bool bounds_check_enabled_ = true;
        uint32_t micros_last_ = 0;
        int32_t velocity_setp_ = 0;
        int32_t velocity_curr_ = 0;

        int32_t min_position_ = 0;
        int32_t max_position_ = 0;
        uint32_t max_speed_ = 0;
        uint32_t max_accel_ = 0;
};

#endif


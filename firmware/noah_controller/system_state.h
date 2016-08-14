#ifndef SYSTEM_STATE_H
#define SYSTEM_STATE_H
#include "Arduino.h"
#include "constants.h"

class SystemState
{
    public:
        volatile bool new_msg_flag_;

        SystemState();
        void initialize();
        void update();

    private:
        constants::OperatingMode mode_;
        IntervalTimer timer_;
        uint64_t time_us_ = 0;
        uint32_t micros_last_ = 0;

        void setup_analog_input();
        void setup_trigger_output();
        void setup_digital_output();
        void setup_pwm_output();
        void setup_timer();

};

extern SystemState system_state;

#endif 

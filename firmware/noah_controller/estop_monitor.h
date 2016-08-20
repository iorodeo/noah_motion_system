#ifndef ESTOP_MONITOR_H
#define ESTOP_MONITOR_H
#include <Arduino.h>

class EStopMonitor
{
    public:

        EStopMonitor(uint8_t pin=A0, uint16_t threshold=20000, uint32_t startup_delay=50);

        uint8_t analog_pin();
        void set_analog_pin(uint8_t pin);

        uint16_t threshold();
        void set_threshold(uint16_t threshold);

        uint32_t startup_delay();
        void set_startup_delay(uint32_t startup_delay);
        void zero_startup_count();

        bool is_stopped();

    protected:

        uint8_t  pin_;
        uint16_t pin_volt_;
        uint16_t threshold_;

        uint32_t startup_delay_;
        uint32_t startup_count_;

};


#endif

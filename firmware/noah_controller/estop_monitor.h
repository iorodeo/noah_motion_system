#ifndef ESTOP_MONITOR_H
#define ESTOP_MONITOR_H
#include <Arduino.h>

class EStopMonitor
{
    public:

        EStopMonitor(uint8_t pin=A0, uint16_t threshold=20000);

        uint8_t analog_pin();
        void set_analog_pin(uint8_t pin);

        uint16_t threshold();
        void set_threshold(uint16_t threshold);

        bool is_stopped();

    protected:

        uint8_t  pin_;
        uint16_t pin_volt_;
        uint16_t threshold_;
};


#endif

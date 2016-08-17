#ifndef TRIGGER_H
#define TRIGGER_H
#include <Arduino.h>

class Trigger
{
    public:
        Trigger(uint8_t pin=0, uint16_t count=5000, bool enabled=false);
        void initialize();

        void setCount(uint16_t count);
        uint16_t getCount();

        void setEnabled(bool enabled);
        bool getEnabled();

        inline void set_pin_high_on_count(uint32_t timer_count);
        inline void set_pin_low_on_count(uint32_t time_count);

    private:
        uint8_t pin_ = 0;
        bool enabled_ = false;
        uint16_t count_ = 5000;
};

inline void Trigger::set_pin_high_on_count(uint32_t timer_count)
{
    if (enabled_ && (timer_count%count_ == 0))
    {
        digitalWriteFast(pin_, HIGH);
    }
}

inline void Trigger::set_pin_low_on_count(uint32_t timer_count)
{
    if (enabled_ && (timer_count%count_ == 0))
    {
        digitalWriteFast(pin_, LOW);
    }
}

#endif


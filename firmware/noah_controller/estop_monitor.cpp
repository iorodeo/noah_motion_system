#include "estop_monitor.h"

EStopMonitor::EStopMonitor(uint8_t pin, uint16_t threshold, uint32_t startup_delay)
{
    set_analog_pin(pin);
    set_threshold(threshold);
    set_startup_delay(startup_delay);
    zero_startup_count();
}

uint8_t EStopMonitor::analog_pin()
{
    return pin_;
}


void EStopMonitor::set_analog_pin(uint8_t pin)
{
    pin_ = pin;
}

uint16_t EStopMonitor::threshold()
{
    return threshold_;
}


void EStopMonitor::set_threshold(uint16_t threshold)
{
    threshold_ = threshold;
}

bool EStopMonitor::is_stopped()
{
    bool rval = false;
    if (startup_count_ >= startup_delay_)
    {
        if (analogRead(pin_) > threshold_)
        {
            return true;
        }
    }
    else
    {
        startup_count_++;
    }
    return rval;
}


uint32_t EStopMonitor::startup_delay()
{
    return startup_delay_;
}

void EStopMonitor::set_startup_delay(uint32_t startup_delay)
{
    startup_delay_ = startup_delay;
}

void EStopMonitor::zero_startup_count()
{
    startup_count_ = 0;
}


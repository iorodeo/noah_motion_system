#include "estop_monitor.h"

EStopMonitor::EStopMonitor(uint8_t pin, uint16_t threshold)
{
    set_analog_pin(pin);
    set_threshold(threshold);
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
    if (analogRead(pin_) > threshold_)
    {
        return true;
    }
    else
    {
        return false;
    }
}



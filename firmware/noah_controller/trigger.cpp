#include "trigger.h"
#include <util/atomic.h>

Trigger::Trigger(uint8_t pin, uint16_t count, bool enabled)
{
    pin_ = pin;
    count_ = count;
    enabled_ = enabled;
}

void Trigger::initialize()
{
    pinMode(pin_,OUTPUT);
    digitalWrite(pin_,LOW);
}

void Trigger::set_count(uint16_t count)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {    
        count_ = count;
    }
}

uint16_t Trigger::count()
{
    return count_;
}

void Trigger::set_enabled(bool enabled)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) 
    {
        enabled_ = enabled;
    }
}

bool Trigger::enabled()
{
    return enabled_;
}


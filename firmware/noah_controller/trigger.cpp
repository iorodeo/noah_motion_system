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

void Trigger::setCount(uint16_t count)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {    
        count_ = count;
    }
}

uint16_t Trigger::getCount()
{
    return count_;
}

void Trigger::setEnabled(bool enabled)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) 
    {
        enabled_ = enabled;
    }
}

bool Trigger::getEnabled()
{
    return enabled_;
}

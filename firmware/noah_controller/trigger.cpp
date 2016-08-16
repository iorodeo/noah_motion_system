#include "trigger.h"

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
    count_ = count;
}

uint16_t Trigger::getCount()
{
    return count_;
}

void Trigger::setEnabled(bool enabled)
{
    enabled_ = enabled;
}

bool Trigger::getEnabled()
{
    return enabled_;
}

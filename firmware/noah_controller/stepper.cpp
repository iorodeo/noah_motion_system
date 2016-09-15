#include "stepper.h"
#include "dev_constants.h"
#include <util/atomic.h>


Stepper::Stepper(uint8_t clk_pin, uint8_t dir_pin)
{
    clk_pin_ = clk_pin;
    dir_pin_ = dir_pin;
}


void Stepper::initialize()
{
    pinMode(clk_pin_, OUTPUT);
    pinMode(dir_pin_, OUTPUT);
    digitalWrite(clk_pin_,LOW);
    digitalWrite(dir_pin_,LOW);
    last_us_ = micros();
}


int32_t Stepper::position()
{
    int32_t position_local = 0;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        position_local = position_;
    }
    return position_local;
}


void Stepper::set_position(int32_t position)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    { 
        position_ = position;
    }
}


int32_t Stepper::velocity()
{
    return velocity_;
}

void Stepper::set_velocity(int32_t velocity)
{
    int32_t velocity_adj = constrain(velocity,-int32_t(max_speed_), int32_t(max_speed_));
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        velocity_ = velocity_adj;
        step_us_ = uint32_t(1000000l/abs(velocity_adj));
    }
}


int32_t Stepper::max_position()
{
    return max_position_;
}


void Stepper::set_max_position(int32_t position)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        max_position_ = position;
    }
}


int32_t Stepper::min_position()
{
    return min_position_;
}


void Stepper::set_min_position(int32_t position)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        min_position_ = position;
    }
}


uint32_t Stepper::max_speed()
{
    return max_speed_;
}


void Stepper::set_max_speed(uint32_t speed)
{
    max_speed_ = speed;
}


bool Stepper::is_bounds_check_enabled()
{
    return bounds_check_enabled_;
}


void Stepper::enable_bounds_check()
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        bounds_check_enabled_ = true;
    }
}


void Stepper::disable_bounds_check()
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        bounds_check_enabled_ = false;
    }
}




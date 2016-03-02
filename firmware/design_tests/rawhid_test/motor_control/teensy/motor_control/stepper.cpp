#include "stepper.h"
#include "constants.h"
#include <util/atomic.h>
#include <cmath>


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
    velocity_ = 0.0;
    step_cnt_ = 0;
    position_ = 0;
}


int32_t Stepper::position()
{
    return position_;
}


void Stepper::set_position(int32_t position)
{
    position_ = position;
}


float Stepper::velocity()
{
    return velocity_;
}


void Stepper::set_velocity(float velocity)
{
    velocity_ = velocity;
    step_cnt_ = uint32_t(1.0e6/fabs(velocity));
    //step_cnt_ = (1.0e6/velocity)/float(constants::TimerPeriod);
    //ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    //{
    //    step_cnt_ = (1.0e6/velocity)/float(constants::TimerPeriod);
    //}
}


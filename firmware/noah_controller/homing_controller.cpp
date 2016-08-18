#include "homing_controller.h"
#include <util/atomic.h>

// Homing Pin Interrupt Callback
// --------------------------------------------------------------------------------------

void homing_callback()
{
    HomingController::disable();
    HomingController::set_home_found(true);
    //Serial.println("cb");
}

// HomingController static member data
// --------------------------------------------------------------------------------------
bool HomingController::home_found_ = true;;
uint8_t HomingController::pin_ = 0;

// HomingController Public Static Methods
// --------------------------------------------------------------------------------------
void HomingController::initialize(uint8_t pin)
{
    pin_ = pin;
    pinMode(pin_,INPUT);
}


bool HomingController::home_found()
{
    bool home_found_local;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        home_found_local = home_found_;
    }
    return home_found_local;
}

void HomingController::set_home_found(bool value)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        home_found_ = value;
    }
}

void HomingController::enable()
{
    set_home_found(false);
    attachInterrupt(pin_,homing_callback,FALLING);
}

void HomingController::disable()
{
    detachInterrupt(pin_);
}

// HomingController Public Methods
// --------------------------------------------------------------------------------------

HomingController::HomingController(int8_t direction, uint32_t speed)
{
    if (direction > 0)
    {
        velocity_controller_.set_velocity_setp( int32_t(speed));
    }
    else
    {
        velocity_controller_.set_velocity_setp(-int32_t(speed));
    }

    velocity_controller_.disable_bounds_check();
} 


void HomingController::update(int32_t position)
{
    if (home_found_)
    {
        velocity_controller_.set_velocity_setp(0);
    }
    velocity_controller_.update(position);
}


void HomingController::reset()
{
    velocity_controller_.reset();
}


int32_t HomingController::velocity()
{
    return velocity_controller_.velocity();
}


uint32_t HomingController::max_speed()
{
    return velocity_controller_.max_speed();
}


void HomingController::set_max_speed(uint32_t value)
{
    velocity_controller_.set_max_speed(value);
}


uint32_t HomingController::accel()
{
    return velocity_controller_.max_accel();
}


void HomingController::set_accel(uint32_t value)
{
    velocity_controller_.set_max_accel(value);
}


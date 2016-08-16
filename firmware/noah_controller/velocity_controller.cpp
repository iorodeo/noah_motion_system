#include "velocity_controller.h"


VelocityController::VelocityController() {} 

void VelocityController::initialize()
{
    micros_last_ = micros();
}

int32_t VelocityController::velocity_setp()
{
    return velocity_setp_;
}

void VelocityController::set_velocity_setp(int32_t vel)
{
    velocity_setp_ = vel;
}


int32_t VelocityController::velocity_curr()
{
    return velocity_curr_;
}


void VelocityController::set_velocity_curr(int32_t vel)
{
    velocity_curr_ = vel;
}


int32_t VelocityController::min_position()
{
    return min_position_;
}


void VelocityController::set_min_position(int32_t pos)
{
    min_position_ = pos;
}


int32_t VelocityController::max_position()
{
    return max_position_;
}


void VelocityController::set_max_position(int32_t pos)
{
    max_position_ = pos;

}


uint32_t VelocityController::max_speed()
{
    return max_speed_;
}


void VelocityController::set_max_speed(uint32_t speed)
{
    max_speed_ = speed;
}


uint32_t VelocityController::max_accel()
{
    return max_accel_;
}


void VelocityController::set_max_aceel(uint32_t accel)
{
    max_accel_ = accel;
}


void VelocityController::update(int32_t position)
{
    //uint32_t micros_curr = micros();
    //uint32_t dt = micros_curr - micros_last_;

    //if (velocity_setp_ > 0)
    //{
    //    int32_t dist = abs(max_position_ - position);
    //}
    //if (velocity_setp_ < 0)
    //{
    //    int32_t dist = abs(min_position_ - position); 

    //}

    //int32_t velocity_diff = velocity_setp_ - velocity_curr_;
    //int32_t mag_velocity_diff = abs(velocity_diff);

    //if (velocity_diff < 0)
    //{
    //    velocity_curr_= min(velocity_setp_, velocity_curr_ - dt*max_accel_);
    //}

    //if (velocity_diff > 0)
    //{
    //    velocity_curr_ = max(velocity_setp_, velocity_curr_ + dt*max_accel_);

    //}
    //micros_last_ = micros_curr;
}



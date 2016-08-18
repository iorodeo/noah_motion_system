#include "velocity_controller.h"


VelocityController::VelocityController() {} 

void VelocityController::reset()
{
    is_first_ = true;
    velocity_curr_ = 0;
}


int32_t VelocityController::velocity()
{
    return velocity_curr_;
}

void VelocityController::set_velocity(int32_t vel)
{
    velocity_curr_ = vel;
}


int32_t VelocityController::velocity_setp()
{
    return velocity_setp_;
}


void VelocityController::set_velocity_setp(int32_t vel)
{
    velocity_setp_ = vel;
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


void VelocityController::set_max_accel(uint32_t accel)
{
    max_accel_ = accel;
}


void VelocityController::enable_bounds_check()
{
    bounds_check_enabled_ = true;
}


void VelocityController::disable_bounds_check()
{
    bounds_check_enabled_ = false;
}


void VelocityController::update(int32_t position)
{
    uint32_t micros_curr = micros();

    if (is_first_)
    {
        is_first_ = false;
    }
    else
    {
        uint32_t dt = micros_curr - micros_last_;

        // Adjust set point velocity so as to not to exceed maximum acceleration at boundaries
        // i.e. slow as boundary is approached. 
        int32_t velocity_setp_adj = velocity_setp_; 

        if (bounds_check_enabled_)
        {
            if (velocity_setp_  > 0)
            {
                int32_t dist = max_position_ - position;
                if (dist > 0) 
                {
                    int32_t stopping_dist = abs(velocity_setp_*velocity_setp_)/int32_t(max_accel_);
                    if (dist < stopping_dist);
                    {
                        velocity_setp_adj = min(velocity_setp_, int32_t(sqrt(2.0*max_accel_*dist)));
                    }
                }
                else
                {
                    // NOTE: might want to add correction for overshoot?
                    velocity_setp_adj = 0;
                }
            }
            if (velocity_setp_ < 0)
            {
                int32_t dist = position - min_position_;
                if (dist > 0)
                {
                    int32_t stopping_dist = abs(velocity_setp_*velocity_setp_)/int32_t(max_accel_);
                    if (dist < stopping_dist)
                    {
                        velocity_setp_adj = max(velocity_setp_, -int32_t(sqrt(2.0*max_accel_*dist)));
                    }
                }
                else
                {
                    // NOTE: might want to add correction for overshoot?
                    velocity_setp_adj = 0;
                }
            }
        }

        // If not at the set point velocity accelerate towards at maximum acceleration,  but don't step past 
        // the set point velocity.
        int32_t velocity_diff = velocity_setp_adj - velocity_curr_;
        if (velocity_diff < 0)
        {
            velocity_curr_ -=  int32_t((dt*max_accel_)/1000000l);
            if (velocity_curr_ < velocity_setp_adj)
            { 
                velocity_curr_ = velocity_setp_adj;
            }
        }
        else
        {
            velocity_curr_ += int32_t((dt*max_accel_)/1000000l);
            if (velocity_curr_ > velocity_setp_adj)
            {
                velocity_curr_ = velocity_setp_adj;
            }
        }
    }

    micros_last_ = micros_curr;
}




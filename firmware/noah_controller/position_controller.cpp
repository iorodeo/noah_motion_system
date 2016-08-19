#include "position_controller.h"


PositionController::PositionController() {};

int32_t PositionController::position_setp()
{
    return position_setp_;
}


void PositionController::set_position_setp(int32_t position)
{
    position_setp_ = position;
}


uint32_t PositionController::gain()
{
    return gain_;
}


void PositionController::set_gain(uint32_t gain)
{
    gain_ = gain;
}


uint32_t PositionController::max_speed()
{
    return max_speed_;
}


void PositionController::set_max_speed(uint32_t speed)
{
    max_speed_ = speed;
}


int32_t PositionController::velocity()
{
    return velocity_;
}


int32_t PositionController::update(int32_t position)
{
    int32_t error = position_setp_ - position;
    int32_t control = gain_*error;
    velocity_ = constrain(control,-int32_t(max_speed_), int32_t(max_speed_)); 
    return velocity_;
}




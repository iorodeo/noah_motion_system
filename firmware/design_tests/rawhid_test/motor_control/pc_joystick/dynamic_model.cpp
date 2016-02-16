#include "dynamic_model.hpp"


DynamicModel::DynamicModel(double mass, double damping)
{
    mass_ = mass;
    damping_ = damping;
    initialize();
}

void DynamicModel::initialize(double position, double velocity)
{
    set_position(position);
    set_velocity(velocity);
}

void DynamicModel::set_position(double position)
{
    position_ = position;
}


void DynamicModel::set_velocity(double velocity)
{
    velocity_ = velocity;
}

double DynamicModel::position()
{
    return position_;
}

double DynamicModel::velocity()
{
    return velocity_;
}

void DynamicModel::update(double force, double dt)
{
    double newVelocity = velocity_ + (dt/mass_)*(force - damping_*velocity_);
    double newPosition = position_ + dt*velocity_;
    position_ = newPosition;
    velocity_ = newVelocity;
}

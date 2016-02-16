/*
Copyright 2016 IO Rodeo Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
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

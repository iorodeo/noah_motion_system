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
#ifndef DYNAMIC_MODEL_HPP
#define DYNAMIC_MODEL_HPP


class DynamicModel
{
    public:
        DynamicModel(double mass=1.0, double damping=0.01, double spring_const=0.0,double spring_zero=0.0);
        void initialize(double position = 0.0, double velocity = 0.0);
        void set_position(double position);
        void set_velocity(double velocity);
        double position();
        double velocity();
        void update(double force, double dt);

    protected:
        double mass_;
        double damping_;
        double spring_const_;
        double spring_zero_;
        double position_;
        double velocity_;

};

#endif

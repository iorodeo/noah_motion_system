#ifndef DYNAMIC_MODEL_HPP
#define DYNAMIC_MODEL_HPP


class DynamicModel
{
    public:
        DynamicModel(double mass=1.0, double damping=0.01);
        void initialize(double position = 0.0, double velocity = 0.0);
        void set_position(double position);
        void set_velocity(double velocity);
        double position();
        double velocity();
        void update(double force, double dt);

    protected:
        double mass_;
        double damping_;
        double position_;
        double velocity_;

};

#endif

#ifndef STEPPER_H
#define STEPPER_H
#include <Arduino.h>

class Stepper
{
    public:
        
        Stepper(uint8_t clk_pin=1, uint8_t dir_pin=2); 
        void initialize();

        int32_t position();
        void set_position(int32_t position);

        int32_t velocity();
        void set_velocity(int32_t velocity);

        int32_t max_position();
        void set_max_position(int32_t position);

        int32_t min_position();
        void set_min_position(int32_t position);

        uint32_t max_speed();
        void set_max_speed(uint32_t speed);

        bool is_bounds_check_enabled();
        void enable_bounds_check();
        void disable_bounds_check();

        inline void update_dir_and_set_clk_high();
        inline void set_clk_low();

    protected:

        uint8_t clk_pin_;
        uint8_t dir_pin_;

        int32_t min_position_ = -10000;
        int32_t max_position_ =  10000;
        uint32_t max_speed_ = 10000;

        volatile bool bounds_check_enabled_ = false;
        volatile int32_t velocity_ = 0;
        volatile uint32_t last_us_ = 0;
        volatile uint32_t step_us_ = 0;
        volatile int32_t position_ = 0;
};

inline void Stepper::update_dir_and_set_clk_high()
{
    uint32_t curr_us = micros();
    if (velocity_ == 0)
    {
        return;
    } 

    if ((curr_us - last_us_) >= step_us_)
    {
        last_us_= curr_us;
        if (velocity_ > 0)
        {
            if (!bounds_check_enabled_ || (position_ < max_position_))
            {
                digitalWriteFast(dir_pin_,HIGH);
                position_++;
                digitalWriteFast(clk_pin_,HIGH);
            }
        }
        else
        {
            if (!bounds_check_enabled_ || (position_ > min_position_))
            {
                digitalWriteFast(dir_pin_,LOW);
                position_--;
                digitalWriteFast(clk_pin_,HIGH);
            }
        }
    }
}

inline void Stepper::set_clk_low()
{
    digitalWriteFast(clk_pin_,LOW);
}

#endif


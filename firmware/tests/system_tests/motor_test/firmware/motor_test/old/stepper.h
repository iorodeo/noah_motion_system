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

        float velocity();
        void set_velocity(float velocity);

        inline void update();

    protected:

        uint8_t clk_pin_;
        uint8_t dir_pin_;

        volatile uint32_t last_us_;
        volatile uint32_t step_cnt_;
        volatile int32_t position_;

        float velocity_;

};

inline void Stepper::update()
{
    uint32_t curr_us = micros();
    if (velocity_ == 0)
    {
        return;
    } 

    if ((curr_us - last_us_) >= step_cnt_)
    {
        last_us_= curr_us;
        if (velocity_ > 0)
        {
            digitalWriteFast(dir_pin_,HIGH);
            position_++;
            digitalWriteFast(clk_pin_,HIGH);
        }
        else
        {
            digitalWriteFast(dir_pin_,LOW);
            position_--;
            digitalWriteFast(clk_pin_,HIGH);
        }
    }

    delayMicroseconds(1);
    digitalWriteFast(clk_pin_,LOW);
}

#endif

#ifndef POSITION_CONTROLLER_H
#define POSITION_CONTROLLER_H
#include <Arduino.h>

class PositionController
{
    public:

        PositionController();

        int32_t position_setp();
        void set_position_setp(int32_t position);

        uint32_t max_speed();
        void set_max_speed(uint32_t speed);

        uint32_t gain();
        void set_gain(uint32_t gain);

        int32_t velocity();

        void update(int32_t position);

    protected:

        int32_t position_setp_ = 0;
        int32_t velocity_ = 0;
        uint32_t max_speed_ = 1000;
        uint32_t gain_ = 1.0;

};

#endif

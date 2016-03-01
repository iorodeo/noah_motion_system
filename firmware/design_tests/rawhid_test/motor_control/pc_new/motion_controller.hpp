#ifndef MOTION_CONTROLLER_HPP
#define MOTION_CONTROLLER_HPP
#include <vector>
#include <string>
#include "trajectory.hpp"
#include "rawhid_device.hpp"

class MotionController
{
    public:
        static const std::string  Default_Output_Filename;

        MotionController(int vid=0, int pid=0);

        bool open();
        bool close();
        bool run();

        void set_output_filename(std::string filename);
        std::string get_output_filename();

        void set_trajectory(std::vector<TrajData> trajectory);
        std::vector<TrajData> get_trajectory();


    protected:

        RawHIDDevice hid_dev_;
        std::string output_filename_;
        std::vector<TrajData> trajectory_;

        bool set_pos_to_start();
        bool set_vel_to_zero();
        bool set_to_vel_mode();

};

#endif

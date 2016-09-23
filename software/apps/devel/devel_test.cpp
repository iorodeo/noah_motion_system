#include "controller.hpp"
#include "rtn_status.hpp"
#include "ft_sensor_cal.hpp"
#include "filesystem/path.h"

#include <iostream>
#include <fstream>


int main(int argc, char *argv[])
{

    mctl::FT_SensorCal cal;
    mctl::RtnStatus rtn_status;

    rtn_status = cal.load(std::string("/home/wbd/.mctl/FT8652.cal"));
    if (!rtn_status.success())
    {
        std::cout << rtn_status.error_msg() << std::endl;
    }

    mctl::Configuration config;
    rtn_status = config.load();

    if (!rtn_status.success())
    {
        std::cout << rtn_status.error_msg() << std::endl;
    }


    //mctl::Controller controller;

    //controller.set_exit_on_error(true);
    //controller.open();
    //controller.set_mode_ready();

    //for (auto num : mctl::StepperList)
    //{ 
    //    controller.set_homed_true(num);
    //}
    //controller.set_display_position_on_move(true);

    //mctl::OutscanData data;
    //controller.outscan("test_trajectory.txt", data);

    //data.save("test_data.h5");

    //controller.close();
    return 0;
}




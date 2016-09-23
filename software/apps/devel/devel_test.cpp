#include "controller.hpp"
#include "rtn_status.hpp"
#include <iostream>
#include <fstream>
#include "filesystem/path.h"


int main(int argc, char *argv[])
{
    mctl::Configuration config;
    mctl::RtnStatus rtn_status = config.load();

    if (!rtn_status.success())
    {
        std::cout << rtn_status.error_msg() << std::endl;
    }

    //json json_config;
    //std::ifstream config_file("temp.json", std::ifstream::in);

    //if (config_file.is_open())
    //{
    //    config_file >> json_config;

    //}
    //else
    //{
    //    std::cout << "couldn't open json file" << std::endl;
    //    return 0;
    //}
    //
    //if (!json_config.is_object())
    //{
    //    std::cout << "error: top level config file must be object" << std::endl;
    //    return 0;
    //}

    //if (json_config.count("ft_calibration"))
    //{
    //    std::cout << json_config["ft_calibration"] << std::endl;

    //}



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




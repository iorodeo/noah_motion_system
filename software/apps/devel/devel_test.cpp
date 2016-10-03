#include "controller.hpp"
#include "rtn_status.hpp"
#include "ft_sensor_cal.hpp"
#include "filesystem/path.h"

#include <iostream>
#include <fstream>


int main(int argc, char *argv[])
{
    mctl::Controller controller;
    controller.open();
    controller.set_exit_on_error(true);
    controller.set_mode_ready();


    double min_pos_in = -0.1;
    double min_pos_out;
    controller.set_min_position(mctl::Axis_X, min_pos_in);
    controller.get_min_position(mctl::Axis_X, min_pos_out);
    std::cout << "min_pos_in: " << min_pos_in << std::endl;
    std::cout << "min_pos_out: " << min_pos_out << std::endl;
    std::cout << std::endl;
    
    double max_pos_in = -0.1;
    double max_pos_out;
    controller.set_max_position(mctl::Axis_X, max_pos_in);
    controller.get_max_position(mctl::Axis_X, max_pos_out);
    std::cout << "max_pos_in: " << max_pos_in << std::endl;
    std::cout << "max_pos_out: " << max_pos_out << std::endl;
    std::cout << std::endl;

    double max_speed_in = 0.1;
    double max_speed_out;
    controller.set_max_speed(mctl::Axis_X, max_speed_in);
    controller.get_max_speed(mctl::Axis_X, max_speed_out);
    std::cout << "max_speed_in: " << max_speed_in << std::endl;
    std::cout << "max_speed_out: " << max_speed_out << std::endl;
    std::cout << std::endl;

    double max_accel_in = 0.1;
    double max_accel_out;
    controller.set_max_accel(mctl::Axis_X, max_accel_in);
    controller.get_max_accel(mctl::Axis_X, max_accel_out);
    std::cout << "max_accel_in: " << max_accel_in << std::endl;
    std::cout << "max_accel_out: " << max_accel_out << std::endl;
    std::cout << std::endl;

    int dir_in = -1;
    int dir_out;
    controller.set_homing_direction(mctl::Axis_X,dir_in);
    controller.get_homing_direction(mctl::Axis_X,dir_out);
    std::cout << "dir_in: " << dir_in << std::endl;
    std::cout << "dir_out: " << dir_out << std::endl;
    std::cout << std::endl;




    //mctl::FT_SensorCal cal;
    //cal.load("/home/wbd/.mctl/FT8652.cal");
    //cal.print_info_string();

    //arma::Row<double> v(7,arma::fill::zeros);
    //v[2] = 0.02;
    //arma::Row<double> f;
    //mctl::RtnStatus rtn_status = cal.convert(v,f);
    //if (!rtn_status.success())
    //{
    //    std::cout << "convert failed" << std::endl;
    //    std::cout << rtn_status.error_msg() << std::endl;
    //}
    //std::cout << v;
    //std::cout << std::endl;
    //std::cout << f;

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




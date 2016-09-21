#include <iostream>
#include "controller.hpp"
#include "configuration.hpp"
#include "ft_sensor_cal.hpp"
#include "ft_tool_transform.hpp"

int main(int argc, char *argv[])
{

    motion::FT_SensorCal ft_sensor_cal;
    motion::RtnStatus rtn_status = ft_sensor_cal.set_from_file("FT8652.cal");
    std::cout << "set_from_file, success      = " << (rtn_status.success()) << std::endl;

    arma::Mat<double> ain_mat(100,6,arma::fill::ones);
    arma::Mat<double> ft_mat;

    ft_sensor_cal.convert(ain_mat,ft_mat);

    std::cout << "ain_mat" << std::endl;
    std::cout << ain_mat;

    std::cout << "ft_mat" << std::endl;
    std::cout << ft_mat;



    //motion::Controller controller;

    //controller.set_exit_on_error(true);
    //controller.open();
    //controller.set_mode_ready();

    //for (auto num : motion::StepperList)
    //{ 
    //    controller.set_homed_true(num);
    //}
    //controller.set_display_position_on_move(true);

    //motion::OutscanData data;
    //controller.outscan("test_trajectory.txt", data);

    //data.save("test_data.h5");

    //controller.close();
    return 0;
}




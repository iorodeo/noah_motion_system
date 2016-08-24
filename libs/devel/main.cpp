#include <iostream>
#include <functional>
#include <armadillo>
#include "motion_controller.hpp"


int main(int argc, char *argv[])
{
    arma::Mat<int32_t> A(10,10,arma::fill::zeros);

    motion::RtnStatus rtn_status;
    motion::Controller controller;

    rtn_status = controller.open();
    if (!rtn_status.success())
    {
        std::cerr << "Error: " << rtn_status.error_msg() << std::endl;
    }

    rtn_status = controller.set_mode_ready();
    if (!rtn_status.success())
    {
        std::cerr << "Error: " << rtn_status.error_msg() << std::endl;
    }

    // Move to position example
    // ----------------------------------------------------------------------------------
    //// Note, stepper axes need to be homed before move
    //for (auto num : motion::StepperList)
    //{
    //    rtn_status = controller.set_homed_true(num);
    //}

    //std::cout << std::endl;
    //std::cout << "start: ";
    //controller.print_position();
    //controller.print_position(false);

    ////std::vector<int32_t> pos_vec = {0,0,0,0,0};
    ////rtn_status = controller.move_to_position(pos_vec);

    //std::map<motion::Axis,int32_t> pos_map = {{motion::Axis_X, 0}, {motion::Axis_Z, 0}};
    //rtn_status = controller.move_to_position(pos_map);

    //if (!rtn_status.success())
    //{
    //    std::cerr << "Error: " << rtn_status.error_msg() << std::endl;
    //}

    //std::cout << std::endl;
    //std::cout << "stop: ";
    //controller.print_position();
    //std::cout << std::endl;


    //// Homing example
    //// ----------------------------------------------------------------------------------
    //std::cout << std::endl;
    //for (auto num : motion::StepperList)
    //{
    //    std::string axis_name = motion::axis_to_string(num);
    //    std::cout << "* homing " << axis_name << " ..." << std::flush;

    //    rtn_status = controller.home(num);
    //    if (!rtn_status.success())
    //    {
    //        std::cerr << "Error: " << rtn_status.error_msg() << std::endl;
    //    }
    //    else
    //    {
    //        std::cout << " done" << std::endl << std::endl;;
    //    }
    //}
    //// ------------------------------------------------------------------------

    controller.close();
    return 0;
}


#include <iostream>
#include <functional>
#include "motion_controller.hpp"

void check_status(motion::Controller &controller, motion::RtnStatus rtn_status);

int main(int argc, char *argv[])
{
    motion::RtnStatus rtn_status;
    motion::Controller controller;

    rtn_status = controller.open();
    check_status(controller,rtn_status);

    rtn_status = controller.set_mode_ready();
    check_status(controller,rtn_status);
    
    // Get mode and print 
    std::string mode_str;
    rtn_status = controller.mode(mode_str);
    check_status(controller,rtn_status);
    std::cout << std::endl;
    std::cout << "mode: " << mode_str << std::endl;
    std::cout << std::endl;

    // Get current index postion
    arma::Row<int32_t> ind_vec;
    rtn_status = controller.position(ind_vec);
    check_status(controller,rtn_status);

    std::cout << "index: ";
    std::cout << ind_vec << std::endl;
    std::cout << std::endl;

    // Get current postion
    arma::Row<double> pos_vec;
    rtn_status = controller.position(pos_vec);
    check_status(controller,rtn_status);

    std::cout << "postion: ";
    std::cout << pos_vec << std::endl;
    std::cout << std::endl;

    // Move to position example
    // ----------------------------------------------------------------------------------
    // Note, stepper axes need to be homed before move
    for (auto num : motion::StepperList)
    {
        rtn_status = controller.set_homed_true(num);
        check_status(controller,rtn_status);
    }

    arma::Row<int32_t> move_ind = {1000,0,0,0,0};
    rtn_status = controller.move_to_position(move_ind);
    check_status(controller,rtn_status);

    // Get current postion
    rtn_status = controller.position(pos_vec);
    check_status(controller,rtn_status);

    std::cout << "postion: ";
    std::cout << pos_vec << std::endl;
    std::cout << std::endl;

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

void check_status(motion::Controller &controller, motion::RtnStatus rtn_status)
{ 
    if (!rtn_status.success())
    {
        std::cerr << "Error: " << rtn_status.error_msg() << std::endl;
        controller.close();
        exit(0);
    }
}

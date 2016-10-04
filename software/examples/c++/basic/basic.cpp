#include <mctl/mctl.hpp>
#include <iostream>

int main(int argc, char *argv[])
{
    mctl::Controller controller;

    // Setup controller
    controller.set_exit_on_error(true);
    controller.load_config();
    controller.open();
    controller.set_mode_ready();
    controller.set_display_position_on_move(true);

    arma::Mat<double> ft;
    controller.get_ft_samples(ft,100);
    std::cout << ft;


    bool ok;
    controller.is_ready_for_outscan(ok);
    if (!ok)
    {
        std::cout << "error: ready for outscan - check mode and homing" << std::endl;
    }

    mctl::OutscanData data;
    controller.outscan("trajectory.txt", data);

    ft = data.force_and_torque();
    std::cout << ft;

    data.save("data.h5");
    controller.close();

    return 0;
}

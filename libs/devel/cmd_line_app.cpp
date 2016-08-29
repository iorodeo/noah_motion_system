#include "docopt.h"
#include "motion_controller.hpp"

#include <string>
#include <iostream>

static const char USAGE[] =
R"(mctrl

    Usage:
      mctrl get-mode  
      mctrl get-position 
      mctrl get-position-ind
      mctrl set-mode-ready
      mctrl set-mode-disabled
      mctrl is-homed
      mctrl home <axis>
      mctrl home-all
      mctrl set-homed <axis>
      mctrl move-to <axis> <value> 
      mctrl move-to-ind <axis> <value>
      mctrl move-to <X> <Y> <Z> <A> <B>
      mctrl move-to-ind <X> <Y> <Z> <A> <B>
      mctrl jog <axis> <value> 
      mctrl jog-ind <axis> <value>
      mctrl outscan <filename>
      mctrl (-h | --help)
      mctrl (-v | --version)

    Options:
      -h --help     Show this screen.
      -v --version  Show version.
)";

int main(int argc, char *argv[])
{
    std::map<std::string, docopt::value> args = docopt::docopt( USAGE, {argv+1,argv+argc}, true, "mctrl 0.1"); 

    for(auto const& arg : args) {
        std::cout << arg.first << ": " << arg.second << std::endl;
    }
    return 0;
}

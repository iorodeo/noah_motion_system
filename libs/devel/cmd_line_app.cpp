#include "docopt.h"
#include "motion_controller.hpp"

#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <utility>

static const char USAGE[] =
R"(mctrl 

    Usage:
      mctrl home AXIS 
      mctrl is-homed
      mctrl home-all
      mctrl set-homed AXIS
      mctrl get-mode  
      mctrl set-mode-ready
      mctrl set-mode-disabled
      mctrl move-to AXIS <value> 
      mctrl move-to-ind AXIS <value>
      mctrl move-to-ind <X> <Y> <Z> <A> <B>
      mctrl jog AXIS <value> 
      mctrl jog-ind AXIS <value>
      mctrl outscan FILENAME 
      mctrl get-dt
      mctrl get-position 
      mctrl get-position-ind
      mctrl help COMMAND 
      mctrl (-h | --help)
      mctrl (-v | --version)

    Options:
      -h --help     Show this screen.
      -v --version  Show version.
)";

//      mctrl move-to <X> <Y> <Z> <A> <B>

std::map<std::string, motion::Axis> StringToAxisMap = 
{
    {"x", motion::Axis_X},
    {"y", motion::Axis_Y},
    {"z", motion::Axis_Z},
    {"a", motion::Axis_A},
    {"b", motion::Axis_B},
};


//void cmd_home(motion::Controller &controller, std::map<std::string,docopt::value> arg_map);

//void cmd_is_homed(motion::Controller &controller, std::map<std::string,docopt::value> arg_map);

//void cmd_home_all(motion::Controller &controller, std::map<std::string,docopt::value> arg_map);

void cmd_set_homed(motion::Controller &controller, std::map<std::string,docopt::value> arg_map);

void cmd_get_mode(motion::Controller &controller, std::map<std::string,docopt::value> arg_map);

void cmd_set_mode_ready(motion::Controller &controller, std::map<std::string,docopt::value> arg_map);

void cmd_set_mode_disabled(motion::Controller &controller, std::map<std::string,docopt::value> arg_map);

void cmd_get_position(motion::Controller &controller, std::map<std::string,docopt::value> arg_map);

void cmd_get_position_ind(motion::Controller &controller, std::map<std::string,docopt::value> arg_map);

void cmd_move_to(motion::Controller &controller, std::map<std::string,docopt::value> arg_map);

//void cmd_move_to_ind(motion::Controller &controller, std::map<std::string,docopt::value> arg_map);

void cmd_jog(motion::Controller &controller, std::map<std::string,docopt::value> arg_map);

//void cmd_jog_ind(motion::Controller &controller, std::map<std::string,docopt::value> arg_map);


int main(int argc, char *argv[])
{
    // Replace '-' before numbers with 'n' and parse command line arguments
    for (int i=0;i<argc;i++)
    {
        if (strlen(argv[i]) > 2)
        {
            if ((argv[i][0] == '-') && std::isdigit(argv[i][1]))
            {
                argv[i][0] = 'n';
            }
        }
        std::cout << argv[i] << std::endl;
    }
    std::map<std::string,docopt::value> arg_map = docopt::docopt(USAGE,{argv+1,argv+argc},true,"mctrl 0.1"); 

    // Display argument map
    if (true)
    {
        std::cout << std::endl;
        std::cout << "arg_map" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        for(auto const& arg : arg_map) {
            std::cout << arg.first << ": " << arg.second << std::endl;
        }
        std::cout << "----------------------------------------" << std::endl;
        std::cout << std::endl;
    }

    motion::Controller controller;
    controller.set_exit_on_error(true);
    controller.set_display_position_on_move(true);
    controller.open();

    // Run command based on argument
    if (arg_map["home"] == docopt::value(true))
    {
    }
    else if (arg_map["is-homed"] == docopt::value(true))
    {
    }
    else if (arg_map["home-all"] == docopt::value(true))
    {
    }
    else if (arg_map["set-homed"] == docopt::value(true))
    {
        cmd_set_homed(controller,arg_map);
    }
    else if (arg_map["get-mode"] == docopt::value(true))
    {
        cmd_get_mode(controller,arg_map);
    }
    else if (arg_map["set-mode-ready"] == docopt::value(true))
    {
        cmd_set_mode_ready(controller,arg_map);
    }
    else if (arg_map["set-mode-disabled"] == docopt::value(true))
    {
        cmd_set_mode_disabled(controller,arg_map);
    }
    else if (arg_map["get-position"] == docopt::value(true))
    {
        cmd_get_position(controller,arg_map);

    }
    else if (arg_map["get-position-ind"] == docopt::value(true))
    {
        cmd_get_position_ind(controller,arg_map);
    }
    else if (arg_map["move-to"] == docopt::value(true))
    {
        cmd_move_to(controller,arg_map);
    }
    else if (arg_map["jog"] == docopt::value(true))
    {
        cmd_jog(controller,arg_map);
    }

    controller.close();
    return 0;
}


void cmd_set_homed(motion::Controller &controller, std::map<std::string,docopt::value> arg_map)
{
    // Get axis string and convert to lower cast
    std::string axis_str = arg_map["AXIS"].asString();
    std::transform(axis_str.begin(), axis_str.end(),axis_str.begin(),::tolower);

    // Get motion::Axis 
    if (StringToAxisMap.count(axis_str) == 0)
    {
        if (axis_str.compare(std::string("all")) == 0)
        {
            std::cout << "setting home=true for all axes" << std::endl;
            for (motion::Axis ax : motion::StepperList)
            {
                controller.set_homed_true(ax);
            }
        }
        else
        {
            std::cout << "error: axis name not recognized" << std::endl;
            return;
        }
    }
    else
    {
        std::cout << "setting home=true for " << axis_str << "axis" << std::endl;
        motion::Axis axis = StringToAxisMap[axis_str];
        controller.set_homed_true(axis);
    }
}


void cmd_get_mode(motion::Controller &controller, std::map<std::string,docopt::value> arg_map)
{
    std::string mode_str;
    controller.mode(mode_str);
    std::cout << "mode: " << mode_str << std::endl;
}


void cmd_set_mode_ready(motion::Controller &controller, std::map<std::string,docopt::value> arg_map)
{
    controller.set_mode_ready();
    std::cout << "setting mode to ready" << std::endl;
}


void cmd_set_mode_disabled(motion::Controller &controller, std::map<std::string,docopt::value> arg_map)
{
    controller.set_mode_disabled();
    std::cout << "setting mode to disabled" << std::endl;
}


void cmd_get_position(motion::Controller &controller, std::map<std::string,docopt::value> arg_map)
{
    std::vector<double> pos_vec;
    controller.position(pos_vec);

    std::vector<std::string> axis_string_list;
    for (auto ax : motion::StepperList)
    {
        std::stringstream ss;
        ss << controller.axis_name(ax) << " (" << controller.axis_unit_string(ax) << ")";
        axis_string_list.push_back(ss.str());
        std::cout << ss.str() << "  "; 
    }
    std::cout << std::endl;

    std::streamsize original_precision = std::cout.precision();
    std::cout << std::setprecision(4);
    std::cout << std::fixed;

    for (int i=0; i<motion::NumStepper; i++)
    {
        std::cout << std::setw(axis_string_list[i].size()) << pos_vec[i]; 
        std::cout << "  ";
    }
    std::cout << std::setprecision(original_precision);
    std::cout << std::endl;
}


void cmd_get_position_ind(motion::Controller &controller, std::map<std::string,docopt::value> arg_map)
{
    std::vector<int32_t> pos_vec;
    controller.position(pos_vec);

    std::vector<std::string> axis_string_list;
    for (auto ax : motion::StepperList)
    {
        std::stringstream ss;
        ss << controller.axis_name(ax);;
        axis_string_list.push_back(ss.str());
        std::cout << ss.str() << "  "; 
    }
    std::cout << std::endl;

    for (int i=0; i<motion::NumStepper; i++)
    {
        std::cout << std::setw(axis_string_list[i].size()) << pos_vec[i]; 
        std::cout << "  ";
    }
    std::cout << std::endl;
}


void cmd_move_to(motion::Controller &controller, std::map<std::string,docopt::value> arg_map)
{
    // Get axis string and convert to lower cast
    std::string axis_str = arg_map["AXIS"].asString();
    std::transform(axis_str.begin(), axis_str.end(),axis_str.begin(),::tolower);

    // Get motion::Axis 
    if (StringToAxisMap.count(axis_str) == 0)
    {
        std::cout << "error: axis name not recognized" << std::endl;
        return;
    }
    motion::Axis axis = StringToAxisMap[axis_str];

    // Get value
    std::string value_str = arg_map["<value>"].asString();
    if (value_str[0] == 'n')
    {
        value_str[0] = '-';
    }
    double value = std::stod(value_str);

    controller.move_to_position(axis,value);
}


void cmd_jog(motion::Controller &controller, std::map<std::string,docopt::value> arg_map)
{
    // Get axis string and convert to lower cast
    std::string axis_str = arg_map["AXIS"].asString();
    std::transform(axis_str.begin(), axis_str.end(),axis_str.begin(),::tolower);

    // Get motion::Axis 
    if (StringToAxisMap.count(axis_str) == 0)
    {
        std::cout << "error: axis name not recognized" << std::endl;
        return;
    }
    motion::Axis axis = StringToAxisMap[axis_str];

    // Get value
    std::string value_str = arg_map["<value>"].asString();
    if (value_str[0] == 'n')
    {
        value_str[0] = '-';
    }
    double value = std::stod(value_str);

    controller.jog_position(axis,value);
}

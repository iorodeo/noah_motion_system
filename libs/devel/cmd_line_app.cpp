#include "docopt.h"
#include "motion_controller.hpp"

#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <utility>
#include <functional>

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
      mctrl move-to <X> <Y> <Z> <A> <B>
      mctrl move-to-ind AXIS <value>
      mctrl move-to-ind <X> <Y> <Z> <A> <B>
      mctrl jog AXIS <value> 
      mctrl jog <X> <Y> <Z> <A> <B>
      mctrl jog-ind AXIS <value>
      mctrl outscan FILENAME 
      mctrl get-position 
      mctrl get-position-ind
      mctrl get-config
      mctrl help COMMAND 
      mctrl (-h | --help)
      mctrl (-v | --version)

    Options:
      -h --help     Show this screen.
      -v --version  Show version.
)";


std::map<std::string, motion::Axis> StringToAxisMap = 
{
    {"x", motion::Axis_X},
    {"y", motion::Axis_Y},
    {"z", motion::Axis_Z},
    {"a", motion::Axis_A},
    {"b", motion::Axis_B},
};

std::vector<std::string> AxisValueArgStringList = {"<X>", "<Y>", "<Z>", "<A>", "<B>"};

std::map<std::string,docopt::value>  get_arg_map(int argc, char *argv[]); 

void print_arg_map(std::map<std::string,docopt::value> arg_map);

void cmd_home(motion::Controller &controller, std::map<std::string,docopt::value> arg_map);

void cmd_is_homed(motion::Controller &controller, std::map<std::string,docopt::value> arg_map);

void cmd_home_all(motion::Controller &controller, std::map<std::string,docopt::value> arg_map);

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

void cmd_outscan(motion::Controller &controller, std::map<std::string,docopt::value> arg_map);

double get_docopt_value_as_double(docopt::value docopt_value);


std::map<std::string,std::function<void(motion::Controller&,std::map<std::string,docopt::value>)>> CmdStringToFuncMap = 
{
    {"home", &cmd_home},
    {"is-homed", &cmd_is_homed},
    {"home-all", &cmd_home_all},
    {"set-homed", &cmd_set_homed},
    {"get-mode", &cmd_get_mode},
    {"set-mode-ready",   &cmd_set_mode_ready},
    {"set-mode-disabled", &cmd_set_mode_disabled},
    {"get-position", &cmd_get_position},
    {"get-position-ind", &cmd_get_position_ind},
    {"move-to", &cmd_move_to},
    {"jog", &cmd_jog},
    {"outscan", &cmd_outscan},
};


int main(int argc, char *argv[])
{
    std::map<std::string,docopt::value>  arg_map =  get_arg_map(argc, argv); 

    // DEBUG -display argument map
    if (false) { print_arg_map(arg_map); }

    // Setup motion controller
    motion::Controller controller;
    controller.set_exit_on_error(true);
    controller.set_display_position_on_move(true);
    controller.open();

    // Run command base on command string
    for (auto kv : CmdStringToFuncMap)
    {
        if (arg_map.count(kv.first)!=0)
        {
            if (arg_map[kv.first].asBool())
            {
                kv.second(controller,arg_map);
            }
        }
        else
        {
            std::cout << "error: unknown command string" << std::endl;
        }
    }

    controller.close();
    return 0;
}


std::map<std::string,docopt::value>  get_arg_map(int argc, char *argv[]) 
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
    }
    // Parse command line arguments
    std::map<std::string,docopt::value> arg_map = docopt::docopt(USAGE,{argv+1,argv+argc},true,"mctrl 0.1"); 
    return arg_map;
}


void print_arg_map(std::map<std::string,docopt::value> arg_map)
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


void cmd_home(motion::Controller &controller, std::map<std::string,docopt::value> arg_map)
{
    // Get axis string and convert to lower cast
    std::string axis_str = arg_map["AXIS"].asString();
    std::transform(axis_str.begin(), axis_str.end(),axis_str.begin(),::tolower);
    if (StringToAxisMap.count(axis_str) != 0)
    {
        motion::Axis axis = StringToAxisMap[axis_str];
        controller.home(axis);
    }
    else
    {
        std::cout << "error: unknown axis " << axis_str << std::endl;
    }
}


void cmd_is_homed(motion::Controller &controller, std::map<std::string,docopt::value> arg_map)
{
}


void cmd_home_all(motion::Controller &controller, std::map<std::string,docopt::value> arg_map)
{
    for (auto axis : motion::StepperList)
    {
        std::cout << std::endl;
        std::cout << "homing " << controller.axis_name(axis) << std::cout;
        controller.home(axis);
        std::cout << std::endl;
    }
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
    if (arg_map["AXIS"].isString())
    {
        // Get axis string and convert to lower cast
        std::string axis_str = arg_map["AXIS"].asString();
        std::transform(axis_str.begin(), axis_str.end(),axis_str.begin(),::tolower);

        // Get motion::Axis and position value
        if (StringToAxisMap.count(axis_str) == 0)
        {
            std::cout << "error: axis name not recognized" << std::endl;
            return;
        }
        motion::Axis axis = StringToAxisMap[axis_str];
        double value = get_docopt_value_as_double(arg_map["<value>"]);
        controller.move_to_position(axis,value);
    }
    else
    {
        std::vector<double> pos_vec;
        for (auto arg_str : AxisValueArgStringList)
        {
            double value = get_docopt_value_as_double(arg_map[arg_str]);
            pos_vec.push_back(value);
        }
        controller.move_to_position(pos_vec);
    }
}


void cmd_jog(motion::Controller &controller, std::map<std::string,docopt::value> arg_map)
{
    if (arg_map["AXIS"].isString())
    {
        // Get axis string and convert to lower cast
        std::string axis_str = arg_map["AXIS"].asString();
        std::transform(axis_str.begin(), axis_str.end(),axis_str.begin(),::tolower);

        // Get motion::Axis and jog value
        if (StringToAxisMap.count(axis_str) == 0)
        {
            std::cout << "error: axis name not recognized" << std::endl;
            return;
        }
        motion::Axis axis = StringToAxisMap[axis_str];
        double value = get_docopt_value_as_double(arg_map["<value>"]);

        controller.jog_position(axis,value);
    }
    else
    {
        std::vector<double> pos_vec;
        for (auto arg_str : AxisValueArgStringList)
        {
            double value = get_docopt_value_as_double(arg_map[arg_str]);
            pos_vec.push_back(value);
        }
        controller.jog_position(pos_vec);
    }
}


void cmd_outscan(motion::Controller &controller, std::map<std::string,docopt::value> arg_map)
{
    if (arg_map["FILENAME"].isString())
    {
        std::string filename = arg_map["FILENAME"].asString();
        std::cout << "outscanning " << filename << std::endl;
        controller.outscan(filename);
    }
    else
    {
        std::cout << "error: FILENAME must be string" << std::endl;
    }
}


double get_docopt_value_as_double(docopt::value docopt_value)
{
    double value = 0.0;
    if (docopt_value.isString())
    {
        std::string value_str = docopt_value.asString();
        if (value_str[0] == 'n')
        {
            value_str[0] = '-';
        }
        value = std::stod(value_str);
    }
    return  value;
}

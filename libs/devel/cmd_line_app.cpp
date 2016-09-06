#include "docopt.h"
#include "motion_controller.hpp"

#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <utility>
#include <functional>


const int DisplayPositionPrecision = 4;

static const char USAGE[] =
R"(mctl 

    Usage:
      mctl home AXIS 
      mctl is-homed AXIS
      mctl set-homed AXIS
      mctl get-mode  
      mctl set-mode-ready
      mctl set-mode-disabled
      mctl move-to AXIS <value> 
      mctl move-to <X> <Y> <Z> <A> <B>
      mctl move-to-ind AXIS <value>
      mctl move-to-ind <X> <Y> <Z> <A> <B>
      mctl jog AXIS <value> 
      mctl jog <X> <Y> <Z> <A> <B>
      mctl jog-ind AXIS <value>
      mctl outscan FILENAME 
      mctl get-position 
      mctl get-position-ind
      mctl get-config
      mctl status
      mctl help COMMAND 
      mctl (-h | --help)
      mctl (-v | --version)

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

void cmd_status(motion::Controller &controller, std::map<std::string,docopt::value> arg_map);

void cmd_help(motion::Controller &controller, std::map<std::string,docopt::value> arg_map);

double get_docopt_value_as_double(docopt::value docopt_value);


std::map<std::string,std::function<void(motion::Controller&,std::map<std::string,docopt::value>)>> CmdStringToFuncMap = 
{
    {"home", &cmd_home},
    {"is-homed", &cmd_is_homed},
    {"set-homed", &cmd_set_homed},
    {"get-mode", &cmd_get_mode},
    {"set-mode-ready",   &cmd_set_mode_ready},
    {"set-mode-disabled", &cmd_set_mode_disabled},
    {"get-position", &cmd_get_position},
    {"get-position-ind", &cmd_get_position_ind},
    {"move-to", &cmd_move_to},
    {"jog", &cmd_jog},
    {"outscan", &cmd_outscan},
    {"status", &cmd_status},
    {"help", &cmd_help},
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
    // This is to prevent docopt parser from thinking negative numbers are 
    // options. 
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
    std::map<std::string,docopt::value> arg_map = docopt::docopt(USAGE,{argv+1,argv+argc},true,"mctl 0.1"); 
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
        if (axis_str.compare(std::string("all")) == 0)
        {
            std::cout << "setting home=true for all axes" << std::endl;
            for (motion::Axis axis : motion::StepperList)
            {
                std::cout << std::endl;
                std::cout << "homing " << controller.axis_name(axis) << std::cout;
                controller.home(axis);
                std::cout << std::endl;
            }
        }
        else
        {
            std::cout << "error: axis name not recognized" << std::endl;
            return;
        }
    }
}


void cmd_is_homed(motion::Controller &controller, std::map<std::string,docopt::value> arg_map)
{
    // Get axis string and convert to lower cast
    std::string axis_str = arg_map["AXIS"].asString();
    std::transform(axis_str.begin(), axis_str.end(),axis_str.begin(),::tolower);
    
    if (StringToAxisMap.count(axis_str) != 0)
    {
        bool is_homed = false;
        motion::Axis axis = StringToAxisMap[axis_str];
        controller.is_homed(axis,is_homed);
        std::cout << std::boolalpha;
        std::cout << "axis " << axis_str << " homed = " << is_homed <<  std::endl;
        std::cout << std::noboolalpha;
    }
    else
    {
        if (axis_str.compare(std::string("all")) == 0)
        {
            for (const auto num : motion::StepperList)
            {
                bool is_homed = false;
                motion::Axis axis = motion::Axis(num);
                std::string axis_str = controller.axis_name(axis);
                controller.is_homed(axis,is_homed);
                std::cout << std::boolalpha;
                std::cout << "axis " << axis_str << " homed = " << is_homed <<  std::endl;
                std::cout << std::noboolalpha;
            }
        }
    }
}


void cmd_set_homed(motion::Controller &controller, std::map<std::string,docopt::value> arg_map)
{
    // Get axis string and convert to lower cast
    std::string axis_str = arg_map["AXIS"].asString();
    std::transform(axis_str.begin(), axis_str.end(),axis_str.begin(),::tolower);

    if (StringToAxisMap.count(axis_str) != 0)
    {
        std::cout << "setting home = true for " << axis_str << " axis" << std::endl;
        motion::Axis axis = StringToAxisMap[axis_str];
        controller.set_homed_true(axis);
    }
    else
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
        ss << std::left << controller.axis_name(ax) << " (" << controller.axis_unit_string(ax) << ")";
        ss << "     ";
        axis_string_list.push_back(ss.str());
        std::cout << ss.str();  
    }
    std::cout << std::endl;

    std::streamsize original_precision = std::cout.precision();
    std::cout << std::setprecision(DisplayPositionPrecision);
    std::cout << std::fixed;

    for (int i=0; i<motion::NumStepper; i++)
    {
        std::cout << std::left << std::setw(axis_string_list[i].size()) << pos_vec[i]; 
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
        std::string pos_str = std::to_string(pos_vec[ax]);
        std::stringstream ss;
        ss << std::setw(pos_str.size()) << controller.axis_name(ax);
        axis_string_list.push_back(ss.str());
        std::cout << ss.str() << "    ";
    }
    std::cout << std::endl;

    for (int i=0; i<motion::NumStepper; i++)
    {
        std::cout << std::right << std::setw(axis_string_list[i].size()) << pos_vec[i]; 
        std::cout << "    ";
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

        motion::OutscanData data;
        controller.outscan(filename,data);
    }
    else
    {
        std::cout << "error: FILENAME must be string" << std::endl;
    }
}


void cmd_status(motion::Controller &controller, std::map<std::string,docopt::value> arg_map)
{
    arg_map["AXIS"] = docopt::value(std::string("all"));
    std::cout << std::endl;
    std::cout << "               Motion Controller Status               " << std::endl;
    std::cout << "======================================================" << std::endl;
    std::cout << std::endl;
    cmd_get_mode(controller,arg_map);
    std::cout << std::endl;
    cmd_is_homed(controller,arg_map);
    std::cout << std::endl;
    cmd_get_position(controller, arg_map);
    std::cout << std::endl;
}


void cmd_help(motion::Controller &controller, std::map<std::string,docopt::value> arg_map)
{
    std::cout << "help command" << std::endl;
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

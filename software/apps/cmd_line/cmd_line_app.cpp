#include "cmd_line_app.hpp"

#include <sstream>
#include <iostream>
#include <iomanip>
#include <utility>
#include <exception>
#include <functional>


int main(int argc, char *argv[])
{
    std::map<std::string,docopt::value>  arg_map =  get_arg_map(argc, argv); 

    if (true) { print_arg_map(arg_map); } // DEBUG

    // Setup mctl controller
    mctl::Controller controller;
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
                break;
            }
        }
        else
        {
            std::cout << "error: unknown command string ";
            std::cout << kv.first << std::endl;
        }
    }
    controller.close();
    return 0;
}



// Command functions
// ------------------------------------------------------------------------------------------------
void cmd_home(mctl::Controller &controller, std::map<std::string,docopt::value> arg_map)
{
    std::string axis_str("");
    RtnStatus rtn_status = get_axis_str_from_arg_map(arg_map,axis_str);
    if (rtn_status.success())
    {
        mctl::Axis axis = StringToAxisMap[axis_str];
        controller.home(axis);
    }
    else
    {
        if (axis_str.compare(std::string("all")) == 0)
        {
            std::cout << "setting home=true for all axes" << std::endl;
            for (mctl::Axis axis : mctl::StepperList)
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


void cmd_is_homed(mctl::Controller &controller, std::map<std::string,docopt::value> arg_map)
{
    std::string axis_str("");
    RtnStatus rtn_status = get_axis_str_from_arg_map(arg_map,axis_str);
    if (rtn_status.success())
    {
        bool is_homed = false;
        mctl::Axis axis = StringToAxisMap[axis_str];
        controller.is_homed(axis,is_homed);
        std::cout << std::boolalpha;
        std::cout << axis_str << " homed = " << is_homed <<  std::endl;
        std::cout << std::noboolalpha;
    }
    else
    {
        if (axis_str.compare(std::string("all")) == 0)
        {
            for (const auto num : mctl::StepperList)
            {
                bool is_homed = false;
                mctl::Axis axis = mctl::Axis(num);
                std::string axis_str = controller.axis_name(axis);
                controller.is_homed(axis,is_homed);
                std::cout << std::boolalpha;
                std::cout << axis_str << " homed = " << is_homed <<  std::endl;
                std::cout << std::noboolalpha;
            }
        }
    }
}


void cmd_set_homed(mctl::Controller &controller, std::map<std::string,docopt::value> arg_map)
{
    std::string axis_str("");
    RtnStatus rtn_status = get_axis_str_from_arg_map(arg_map,axis_str);

    if (rtn_status.success())
    {
        std::cout << "setting home = true for " << axis_str << " axis" << std::endl;
        mctl::Axis axis = StringToAxisMap[axis_str];
        controller.set_homed_true(axis);
    }
    else
    {
        if (axis_str.compare(std::string("all")) == 0)
        {
            std::cout << "setting home=true for all axes" << std::endl;
            for (mctl::Axis ax : mctl::StepperList)
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


void cmd_get_mode(mctl::Controller &controller, std::map<std::string,docopt::value> arg_map)
{
    std::string mode_str;
    controller.mode(mode_str);
    std::cout << "mode: " << mode_str << std::endl;
}


void cmd_set_mode_ready(mctl::Controller &controller, std::map<std::string,docopt::value> arg_map)
{
    controller.set_mode_ready();
    std::cout << "setting mode to ready" << std::endl;
}


void cmd_set_mode_disabled(mctl::Controller &controller, std::map<std::string,docopt::value> arg_map)
{
    controller.set_mode_disabled();
    std::cout << "setting mode to disabled" << std::endl;
}


void cmd_get_position(mctl::Controller &controller, std::map<std::string,docopt::value> arg_map)
{
    std::vector<double> pos_vec;
    controller.position(pos_vec);

    std::vector<std::string> axis_string_list;
    for (auto ax : mctl::StepperList)
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

    for (int i=0; i<mctl::NumStepper; i++)
    {
        std::cout << std::left << std::setw(axis_string_list[i].size()) << pos_vec[i]; 
    }
    std::cout << std::setprecision(original_precision);
    std::cout << std::endl;
}


void cmd_get_position_ind(mctl::Controller &controller, std::map<std::string,docopt::value> arg_map)
{
    std::vector<int32_t> pos_vec;
    controller.position(pos_vec);

    std::vector<std::string> axis_string_list;
    for (auto ax : mctl::StepperList)
    {
        std::string pos_str = std::to_string(pos_vec[ax]);
        std::stringstream ss;
        ss << std::setw(pos_str.size()) << controller.axis_name(ax);
        axis_string_list.push_back(ss.str());
        std::cout << ss.str() << "    ";
    }
    std::cout << std::endl;

    for (int i=0; i<mctl::NumStepper; i++)
    {
        std::cout << std::right << std::setw(axis_string_list[i].size()) << pos_vec[i]; 
        std::cout << "    ";
    }
    std::cout << std::endl;
}


void cmd_move_to(mctl::Controller &controller, std::map<std::string,docopt::value> arg_map)
{
    RtnStatus rtn_status;
    if (arg_map["<axis>"].isString())
    {
        mctl::Axis axis;
        rtn_status = get_axis_from_arg_map(arg_map,axis);
        if (!rtn_status.success())
        {
            std::cout << rtn_status.error_msg() << std::endl;
            return;
        }
        double value = 0.0; 
        rtn_status = get_docopt_value_as_double(arg_map["<value>"],value);
        if (!rtn_status.success())
        {
            std::cout << rtn_status.error_msg() << std::endl;
            return;
        }
        controller.move_to_position(axis,value);
    }
    else
    {
        std::vector<double> pos_vec;
        for (auto arg_str : AxisValueArgStringList)
        {
            double value = 0.0; 
            rtn_status = get_docopt_value_as_double(arg_map[arg_str],value);
            if (!rtn_status.success())
            {
                std::cout << rtn_status.error_msg() << std::endl;
                return;
            }
            pos_vec.push_back(value);
        }
        controller.move_to_position(pos_vec);
    }
}


void cmd_move_to_ind(mctl::Controller &controller, std::map<std::string,docopt::value> arg_map)
{
    RtnStatus rtn_status;
    if (arg_map["<axis>"].isString())
    {
        mctl::Axis axis;
        
        rtn_status = get_axis_from_arg_map(arg_map,axis);
        if (!rtn_status.success())
        {
            std::cout << rtn_status.error_msg() << std::endl;
            return;
        }
        int32_t value = 0;
        rtn_status = get_docopt_value_as_int32(arg_map["<value>"],value);
        if (!rtn_status.success())
        {
            std::cout << rtn_status.error_msg() << std::endl;
        }
        controller.move_to_position(axis,value);
    }
    else
    {
        std::vector<int32_t> ind_vec;
        for (auto arg_str : AxisValueArgStringList)
        {
            int32_t value = 0; 
            rtn_status = get_docopt_value_as_int32(arg_map[arg_str],value);
            if (!rtn_status.success())
            {
                std::cout << rtn_status.error_msg() << std::endl;
                return;
            }
            ind_vec.push_back(value);
        }
        controller.move_to_position(ind_vec);
    }
}


void cmd_jog(mctl::Controller &controller, std::map<std::string,docopt::value> arg_map)
{
    RtnStatus rtn_status;
    if (arg_map["<axis>"].isString())
    {
        mctl::Axis axis;
        rtn_status = get_axis_from_arg_map(arg_map,axis);
        if (!rtn_status.success())
        {
            std::cout << rtn_status.error_msg() << std::endl;
            return;
        }
        double value = 0.0; 
        rtn_status = get_docopt_value_as_double(arg_map["<value>"],value);
        if (!rtn_status.success())
        {
            std::cout << rtn_status.error_msg() << std::endl;
            return;
        }
        controller.jog_position(axis,value);
    }
    else
    {
        std::vector<double> pos_vec;
        for (auto arg_str : AxisValueArgStringList)
        {
            double value = 0.0; 
            rtn_status = get_docopt_value_as_double(arg_map[arg_str],value);
            if (!rtn_status.success())
            {
                std::cout << rtn_status.error_msg() << std::endl;
                return;
            }
            pos_vec.push_back(value);
        }
        controller.jog_position(pos_vec);
    }
}


void cmd_jog_ind(mctl::Controller &controller, std::map<std::string,docopt::value> arg_map)
{
    RtnStatus rtn_status;
    if (arg_map["<axis>"].isString())
    {
        mctl::Axis axis;
        
        rtn_status = get_axis_from_arg_map(arg_map,axis);
        if (!rtn_status.success())
        {
            std::cout << rtn_status.error_msg() << std::endl;
            return;
        }
        int32_t value = 0;
        rtn_status = get_docopt_value_as_int32(arg_map["<value>"],value);
        if (!rtn_status.success())
        {
            std::cout << rtn_status.error_msg() << std::endl;
        }
        controller.jog_position(axis,value);
    }
    else
    {
        std::vector<int32_t> ind_vec;
        for (auto arg_str : AxisValueArgStringList)
        {
            int32_t value = 0; 
            rtn_status = get_docopt_value_as_int32(arg_map[arg_str],value);
            if (!rtn_status.success())
            {
                std::cout << rtn_status.error_msg() << std::endl;
                return;
            }
            ind_vec.push_back(value);
        }
        controller.jog_position(ind_vec);
    }
}


void cmd_outscan(mctl::Controller &controller, std::map<std::string,docopt::value> arg_map)
{
    if (arg_map["<input_file>"].isString())
    {
        std::string filename = arg_map["<input_file>"].asString();
        std::cout << "outscanning " << filename << std::endl;

        mctl::OutscanData data;
        controller.outscan(filename,data);
    }
    else
    {
        std::cout << "error: <filename> must be string" << std::endl;
    }
}


void cmd_status(mctl::Controller &controller, std::map<std::string,docopt::value> arg_map)
{
    arg_map["<axis>"] = docopt::value(std::string("all"));
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


void cmd_help(mctl::Controller &controller, std::map<std::string,docopt::value> arg_map)
{
    std::cout << "help command" << std::endl;
}


// Utility functions
// ------------------------------------------------------------------------------------------------

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


RtnStatus get_docopt_value_as_double(docopt::value docopt_value, double &value_dbl)
{
    RtnStatus rtn_status;

    if (docopt_value.isString())
    {
        std::string value_str = docopt_value.asString();
        if (value_str[0] == 'n')
        {
            value_str[0] = '-';
        }
        try
        {
            value_dbl = std::stod(value_str);
        }
        catch (std::invalid_argument error)
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("error: unable to convert string to double");
        }
        catch (std::out_of_range error)
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("error: string to double conversion out of range");
        }
    }
    return rtn_status;
}


RtnStatus get_docopt_value_as_int32(docopt::value docopt_value, int32_t  &value_int32)
{
    RtnStatus rtn_status;
    try
    { 
        value_int32 = int32_t(docopt_value.asLong());
    }
    catch (std::runtime_error  error)
    {
        rtn_status.set_success(false);
        rtn_status.set_error_msg("error: unable to convert string to integer");
    }
    catch (std::invalid_argument error)
    {
        rtn_status.set_success(false);
        rtn_status.set_error_msg("error: unable to convert string to int32");
    }
    return rtn_status;
}


RtnStatus get_axis_str_from_arg_map(std::map<std::string,docopt::value> arg_map, std::string &axis_str)
{
    RtnStatus rtn_status;
    if (arg_map["<axis>"].isString())
    {
        axis_str = arg_map["<axis>"].asString();
        std::transform(axis_str.begin(), axis_str.end(),axis_str.begin(),::tolower);

        if (StringToAxisMap.count(axis_str) == 0)
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("error: axis name not recognized");
        }
    }
    else
    {
        rtn_status.set_success(false);
        rtn_status.set_error_msg("error: <axis> argument must be string");
    }
    return rtn_status;
}


RtnStatus get_axis_from_arg_map(std::map<std::string,docopt::value> arg_map, mctl::Axis &axis)
{
    std::string axis_str;
    RtnStatus rtn_status = get_axis_str_from_arg_map(arg_map,axis_str);
    if (rtn_status.success())
    {
        axis = StringToAxisMap[axis_str];
    }
    return rtn_status;
}



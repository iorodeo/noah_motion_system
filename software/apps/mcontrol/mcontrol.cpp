#include "mcontrol.hpp"
#include "mcontrol_help.hpp"

#include <sstream>
#include <iostream>
#include <iomanip>
#include <utility>
#include <exception>
#include <functional>


int main(int argc, char *argv[])
{
    StringToValueMap arg_map =  get_arg_map(argc, argv); 

    if (false) { print_arg_map(arg_map); } // DEBUG

    // Setup mctl controller
    mctl::Controller controller;
    controller.set_exit_on_error(true);
    controller.set_display_position_on_move(true);
    controller.load_config();
    controller.open();

    // Run command base on command string
    int exit_status = 0;
    for (auto kv : CmdStringToFuncMap)
    {
        if (arg_map.count(kv.first)!=0)
        {
            if (arg_map[kv.first].asBool())
            {
                if (!kv.second(controller,arg_map))
                {
                    exit_status = 1;
                }
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
    return exit_status;
}



// Command functions
// ------------------------------------------------------------------------------------------------
bool cmd_home(mctl::Controller &controller, StringToValueMap arg_map)
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
            return false;
        }
    }
    return true;
}


bool cmd_is_homed(mctl::Controller &controller, StringToValueMap arg_map)
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
    return true;
}


bool cmd_set_homed(mctl::Controller &controller, StringToValueMap arg_map)
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
            return false;
        }
    }
    return true;
}


bool cmd_get_mode(mctl::Controller &controller, StringToValueMap arg_map)
{
    std::string mode_str;
    controller.mode(mode_str);
    std::cout << "mode: " << mode_str << std::endl;
    return true;
}


bool cmd_set_mode_ready(mctl::Controller &controller, StringToValueMap arg_map)
{
    controller.set_mode_ready();
    std::cout << "setting mode to ready" << std::endl;
    return true;
}


bool cmd_set_mode_disabled(mctl::Controller &controller, StringToValueMap arg_map)
{
    controller.set_mode_disabled();
    std::cout << "setting mode to disabled" << std::endl;
    return true;
}


bool cmd_get_position(mctl::Controller &controller, StringToValueMap arg_map)
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
    return true;
}


bool cmd_get_position_ind(mctl::Controller &controller, StringToValueMap arg_map)
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
    return true;
}


bool cmd_set_position(mctl::Controller &controller, StringToValueMap arg_map)
{
    RtnStatus rtn_status;
    if (arg_map["<axis>"].isString())
    {
        mctl::Axis axis;
        rtn_status = get_axis_from_arg_map(arg_map,axis);
        if (!rtn_status.success())
        {
            std::cout << rtn_status.error_msg() << std::endl;
            return false;
        }
        double value = 0.0; 
        rtn_status = get_docopt_value_as_double(arg_map["<value>"],value);
        if (!rtn_status.success())
        {
            std::cout << rtn_status.error_msg() << std::endl;
            return false;
        }
        controller.set_position(axis,value);
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
                return false;
            }
            pos_vec.push_back(value);
        }
        controller.set_position(pos_vec);
    }
    return true;
}

bool cmd_set_position_ind(mctl::Controller &controller, StringToValueMap arg_map)
{
    RtnStatus rtn_status;
    if (arg_map["<axis>"].isString())
    {
        mctl::Axis axis;
        
        rtn_status = get_axis_from_arg_map(arg_map,axis);
        if (!rtn_status.success())
        {
            std::cout << rtn_status.error_msg() << std::endl;
            return false;
        }
        int32_t value = 0;
        rtn_status = get_docopt_value_as_int32(arg_map["<value>"],value);
        if (!rtn_status.success())
        {
            std::cout << rtn_status.error_msg() << std::endl;
            return false;
        }
        controller.set_position(axis,value);
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
                return false;
            }
            ind_vec.push_back(value);
        }
        controller.set_position(ind_vec);
    }
    return true;
}


bool cmd_set_trigger_freq(mctl::Controller &controller, StringToValueMap arg_map)
{
    std::vector<int> trigger_vec;
    RtnStatus rtn_status = get_trigger_from_docopt(arg_map["<trigger>"], trigger_vec, false);
    if (!rtn_status.success())
    {
        std::cout << rtn_status.error_msg() << std::endl;;
        return false;
    }

    double frequency;
    rtn_status = get_docopt_value_as_double(arg_map["<freq>"],frequency);
    if (!rtn_status.success())
    {
        std::cout << rtn_status.error_msg() << std::endl;
        return false;
    }
    if (frequency < 0.0)
    {
        std::cout << "error: frequency must be > 0" << std::endl;
        return false;
    }

    std::cout << std::setprecision(DisplayFrequencyPrecision);
    std::cout << std::fixed;
    for (auto trigger : trigger_vec)
    {
        controller.set_trigger_frequency(trigger,frequency);
        std::cout << "triger[" << trigger << "] frequency set to " << frequency << std::endl;
    }
    return true;
}


bool cmd_get_trigger_freq(mctl::Controller &controller, StringToValueMap arg_map)
{
    std::vector<int> trigger_vec;
    RtnStatus rtn_status = get_trigger_from_docopt(arg_map["<trigger>"], trigger_vec);
    if (!rtn_status.success())
    {
        std::cout << rtn_status.error_msg() << std::endl;;
        return false;
    }
    std::cout << std::setprecision(DisplayFrequencyPrecision);
    std::cout << std::fixed;
    for (auto trigger : trigger_vec)
    {
        double frequency;
        controller.get_trigger_frequency(trigger,frequency);
        std::cout << "trigger[" << trigger << "] frequency = " << frequency << std::endl;
    }
    return true;
}


bool cmd_trigger_on(mctl::Controller &controller, StringToValueMap arg_map)
{
    std::vector<int> trigger_vec;
    RtnStatus rtn_status = get_trigger_from_docopt(arg_map["<trigger>"], trigger_vec);
    if (!rtn_status.success())
    {
        std::cout << rtn_status.error_msg() << std::endl;;
        return false;
    }
    for (auto trigger : trigger_vec)
    {
        controller.set_trigger_enabled(trigger,true);
        std::cout << "set trigger[" << trigger << "] on"  << std::endl;
    }
    return true;
}


bool cmd_trigger_off(mctl::Controller &controller, StringToValueMap arg_map)
{
    std::vector<int> trigger_vec;
    RtnStatus rtn_status = get_trigger_from_docopt(arg_map["<trigger>"], trigger_vec);
    if (!rtn_status.success())
    {
        std::cout << rtn_status.error_msg() << std::endl;;
        return false;
    }
    for (auto trigger :trigger_vec)
    {
        controller.set_trigger_enabled(trigger,false);
        std::cout << "set trigger[" << trigger << "] off"  << std::endl;
    }
    return true;
}


bool cmd_trigger_state(mctl::Controller &controller, StringToValueMap arg_map)
{
    std::vector<int> trigger_vec;
    RtnStatus rtn_status = get_trigger_from_docopt(arg_map["<trigger>"], trigger_vec);
    if (!rtn_status.success())
    {
        std::cout << rtn_status.error_msg() << std::endl;;
        return false;
    }
    for (auto trigger : trigger_vec)
    {
        bool enabled;
        controller.get_trigger_enabled(trigger,enabled);
        std::cout << "trigger[" << trigger << "] = " ;
        if (enabled)
        {
            std::cout << "on" << std::endl;
        }
        else
        {
            std::cout << "off" << std::endl;
        }
    }
    return true;
}


bool cmd_get_ai_samples(mctl::Controller &controller, StringToValueMap arg_map)
{
    ///////////////////////////////////////////////////////////////////////
    // NOT DONE
    //////////////////////////////////////////////////////////////////////
    return true;
}


bool cmd_get_ft_samples(mctl::Controller &controller, StringToValueMap arg_map)
{
    ///////////////////////////////////////////////////////////////////////
    // NOT DONE
    //////////////////////////////////////////////////////////////////////
    return true;
}


bool cmd_ai_display(mctl::Controller &controller, StringToValueMap arg_map)
{
    ///////////////////////////////////////////////////////////////////////
    // NOT DONE
    //////////////////////////////////////////////////////////////////////
    return true;
}


bool cmd_ft_display(mctl::Controller &controller, StringToValueMap arg_map)
{
    ///////////////////////////////////////////////////////////////////////
    // NOT DONE
    //////////////////////////////////////////////////////////////////////
    return true;
}


bool cmd_joystick(mctl::Controller &controller, StringToValueMap arg_map)
{
    controller.joystick_pendant();
    return true;
}


bool cmd_set_device_config(mctl::Controller &controller, StringToValueMap arg_map)
{
    std::cout << "setting device configuration ..." << std::flush;
    controller.set_device_config();
    std::cout << "done" << std::endl;
    return true;
}


bool cmd_device_config(mctl::Controller &controller, StringToValueMap arg_map)
{
    std::cout << "reading device configuration ... " << std::flush;
    std::string config_string;
    controller.get_device_config_string(config_string);
    std::cout << "done" << std::endl;
    std::cout << std::endl;
    std::cout << config_string;
    std::cout << std::endl;
}


bool cmd_config_check(mctl::Controller &controller, StringToValueMap arg_map)
{
    ///////////////////////////////////////////////////////////////////////
    // NOT DONE
    //////////////////////////////////////////////////////////////////////
    return true;
}


bool cmd_move_to(mctl::Controller &controller, StringToValueMap arg_map)
{
    RtnStatus rtn_status;
    if (arg_map["<axis>"].isString())
    {
        mctl::Axis axis;
        rtn_status = get_axis_from_arg_map(arg_map,axis);
        if (!rtn_status.success())
        {
            std::cout << rtn_status.error_msg() << std::endl;
            return false;
        }
        double value = 0.0; 
        rtn_status = get_docopt_value_as_double(arg_map["<value>"],value);
        if (!rtn_status.success())
        {
            std::cout << rtn_status.error_msg() << std::endl;
            return false;
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
                return false;
            }
            pos_vec.push_back(value);
        }
        controller.move_to_position(pos_vec);
    }
    return true;
}


bool cmd_move_to_ind(mctl::Controller &controller, StringToValueMap arg_map)
{
    RtnStatus rtn_status;
    if (arg_map["<axis>"].isString())
    {
        mctl::Axis axis;
        
        rtn_status = get_axis_from_arg_map(arg_map,axis);
        if (!rtn_status.success())
        {
            std::cout << rtn_status.error_msg() << std::endl;
            return false;
        }
        int32_t value = 0;
        rtn_status = get_docopt_value_as_int32(arg_map["<value>"],value);
        if (!rtn_status.success())
        {
            std::cout << rtn_status.error_msg() << std::endl;
            return false;
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
                return false;
            }
            ind_vec.push_back(value);
        }
        controller.move_to_position(ind_vec);
    }
    return true;
}


bool cmd_jog(mctl::Controller &controller, StringToValueMap arg_map)
{
    RtnStatus rtn_status;
    if (arg_map["<axis>"].isString())
    {
        mctl::Axis axis;
        rtn_status = get_axis_from_arg_map(arg_map,axis);
        if (!rtn_status.success())
        {
            std::cout << rtn_status.error_msg() << std::endl;
            return false;
        }
        double value = 0.0; 
        rtn_status = get_docopt_value_as_double(arg_map["<value>"],value);
        if (!rtn_status.success())
        {
            std::cout << rtn_status.error_msg() << std::endl;
            return false;
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
                return false;
            }
            pos_vec.push_back(value);
        }
        controller.jog_position(pos_vec);
    }
    return true;
}


bool cmd_jog_ind(mctl::Controller &controller, StringToValueMap arg_map)
{
    RtnStatus rtn_status;
    if (arg_map["<axis>"].isString())
    {
        mctl::Axis axis;
        
        rtn_status = get_axis_from_arg_map(arg_map,axis);
        if (!rtn_status.success())
        {
            std::cout << rtn_status.error_msg() << std::endl;
            return false;
        }
        int32_t value = 0;
        rtn_status = get_docopt_value_as_int32(arg_map["<value>"],value);
        if (!rtn_status.success())
        {
            std::cout << rtn_status.error_msg() << std::endl;
            return false;
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
                return false;
            }
            ind_vec.push_back(value);
        }
        controller.jog_position(ind_vec);
    }
    return true;
}


bool cmd_outscan(mctl::Controller &controller, StringToValueMap arg_map)
{
    // Get input trajectory filename
    if (!arg_map["<input_file>"].isString())
    {
        std::cout << "error: <filename> must be string" << std::endl;
        return false;
    }
    std::string traj_filename = arg_map["<input_file>"].asString();

    // Get output data filename
    std::string data_filename = DefaultOutscanDataFile;
    if (arg_map["-o"])
    {
        if (!arg_map["-o"].isString())
        {
            std::cout << "error: <output_file> must be string" << std::endl;
            return false;
        }
        data_filename = arg_map["-o"].asString();
    }

    // Outscan trajectory
    std::cout << "outscanning " << traj_filename << std::endl;
    mctl::OutscanData data;
    controller.outscan(traj_filename,data);
    mctl::RtnStatus rtn_status = data.save(data_filename);
    if (!rtn_status.success())
    {
        std::cout << rtn_status.error_msg();
        return false;
    }
    return true;
}

bool cmd_is_outscan_ready(mctl::Controller &controller, StringToValueMap arg_map)
{
    bool ready;
    RtnStatus rtn_status = controller.is_ready_for_outscan(ready);
    if (!rtn_status.success())
    {
        std::cout << rtn_status.error_msg();
        return false;
    }
    if (ready)
    {
        std::cout << "ready for outscan" << std::endl;
        return true;
    }
    else
    {
        std::cout << "not ready for outscan" << std::endl;
        return false;
    }
}


bool cmd_status(mctl::Controller &controller, StringToValueMap arg_map)
{
    arg_map["<axis>"] = docopt::value(std::string("all"));
    arg_map["<trigger>"] = docopt::value(std::string("all"));
    std::cout << std::endl;
    std::cout << "               Motion Controller Status               " << std::endl;
    std::cout << "======================================================" << std::endl;
    std::cout << std::endl;
    cmd_get_mode(controller,arg_map);
    std::cout << std::endl;
    cmd_trigger_state(controller,arg_map);
    std::cout << std::endl;
    cmd_get_trigger_freq(controller,arg_map);
    std::cout << std::endl;
    cmd_is_homed(controller,arg_map);
    std::cout << std::endl;
    cmd_get_position(controller, arg_map);
    std::cout << std::endl;
    return true;
}


bool cmd_config_file(mctl::Controller &controller, StringToValueMap arg_map)
{
    mctl::Configuration config = controller.config();

    std::cout << std::endl;
    std::cout << "               Configuration File                     " << std::endl;
    std::cout << "======================================================" << std::endl;
    std::cout << std::endl;
    std::cout << "file: " << config.file() << std::endl;
    std::cout << std::endl;
    std::cout << config.file_contents();
    std::cout << std::endl;
    return true;
}


bool cmd_config_info(mctl::Controller &controller, StringToValueMap arg_map)
{
    mctl::Configuration config = controller.config();

    std::cout << std::endl;
    std::cout << "               Configuration Info                     " << std::endl;
    std::cout << "======================================================" << std::endl;
    std::cout << std::endl;
    std::cout << config.info_string();
    std::cout << std::endl;
    return true;
}


bool cmd_ft_info(mctl::Controller &controller, StringToValueMap arg_map)
{
    mctl::Configuration config = controller.config();

    if (config.have_ft_sensor_cal())
    {
        std::string cal_info = config.ft_sensor_cal_info();
        std::cout << std::endl;
        std::cout << "        Force/Torque Sensor Calibration info          " << std::endl;
        std::cout << "======================================================" << std::endl;
        std::cout << std::endl;
        std::cout << cal_info;
        std::cout << std::endl;

    }
    return true;
}


bool cmd_help(mctl::Controller &controller, StringToValueMap arg_map)
{
    if (!arg_map["<command>"].isString())
    {
        std::cout << "error: <command> must be string" << std::endl;
        return false;
    }
    std::string command = arg_map["<command>"].asString();
    mctl::RtnStatus rtn_status = print_command_help(command);
    if (!rtn_status.success())
    {
        std::cout << rtn_status.error_msg() << std::endl;;
    }
    return true;
}


// Utility functions
// ------------------------------------------------------------------------------------------------

StringToValueMap  get_arg_map(int argc, char *argv[]) 
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
    StringToValueMap arg_map = docopt::docopt(USAGE,{argv+1,argv+argc},true,VersionString); 
    return arg_map;
}


void print_arg_map(StringToValueMap arg_map)
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
        rtn_status.set_error_msg("error: unable to convert string to int32 (*)");
    }
    catch (std::invalid_argument error)
    {
        rtn_status.set_success(false);
        rtn_status.set_error_msg("error: unable to convert string to int32 (**)");
    }
    return rtn_status;
}


RtnStatus get_docopt_value_as_int(docopt::value docopt_value, int &value_int)
{
    RtnStatus rtn_status;
    try
    { 
        value_int= int(docopt_value.asLong());
    }
    catch (std::runtime_error  error)
    {
        rtn_status.set_success(false);
        rtn_status.set_error_msg("error: unable to convert string to int (*)");
    }
    catch (std::invalid_argument error)
    {
        rtn_status.set_success(false);
        rtn_status.set_error_msg("error: unable to convert string to int (**)");
    }
    return rtn_status;
}


RtnStatus get_axis_str_from_arg_map(StringToValueMap arg_map, std::string &axis_str)
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


RtnStatus get_axis_from_arg_map(StringToValueMap arg_map, mctl::Axis &axis)
{
    std::string axis_str;
    RtnStatus rtn_status = get_axis_str_from_arg_map(arg_map,axis_str);
    if (rtn_status.success())
    {
        axis = StringToAxisMap[axis_str];
    }
    return rtn_status;
}

RtnStatus get_trigger_from_docopt(docopt::value docopt_value, std::vector<int> &trigger_vec, bool allow_all)
{
    int trigger_num;
    RtnStatus rtn_status = get_docopt_value_as_int(docopt_value,trigger_num);
    if (rtn_status.success())
    {
        trigger_vec.push_back(trigger_num);
    }
    else
    {
        if (allow_all)
        {
            std::string trigger_str = docopt_value.asString();
            if (trigger_str.compare(std::string("all")) == 0)
            {
                for (int i=0; i<mctl::NumTrigger; i++)
                {
                    trigger_vec.push_back(i);
                }
                rtn_status.set_success(true);
                rtn_status.set_error_msg("");
            }
            else
            {
                rtn_status.set_success(false);
                rtn_status.set_error_msg("error: <trigger> must be # or 'all'");
            }
        }
        else
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("error: <trigger> must be #");
        }
    }
    return rtn_status;
}



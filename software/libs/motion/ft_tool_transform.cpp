#include "ft_tool_transform.hpp"

namespace motion
{
    const std::string FT_SensorCal::DefaultPositionUnits("m");
    const std::string FT_SensorCal::DefaultRotationUnits("degrees");

    FT_ToolTransform::FT_ToolTransform( 
            double dx, 
            double dy, 
            double dz, 
            double ax, 
            double ay, 
            double az,
            std::string position_units,
            std::string rotation_units
            )
    {
        dx_ = dx;
        dy_ = dy;
        dz_ = dz;
        ax_ = ax;
        ay_ = ay;
        az_ = az;
        position_units_ = position_units;
        rotation_units_ = rotation_units;
    }


    void FT_ToolTransform::set_dx(double value)
    {
    }


    void FT_ToolTransform::set_dy(double value)
    {
    }


    void FT_ToolTransform::set_dz(double value)
    {
    }


    void FT_ToolTransform::set_ax(double value)
    {
    }


    void FT_ToolTransform::set_ay(double value)
    {
    }


    void FT_ToolTransform::set_az(double value)
    {
    }


    void FT_ToolTransform::set_position_units(std::string units)
    {
    }


    void FT_ToolTransform::set_rotation_units(std::string units)
    {
    }


    void FT_ToolTransform::dx()
    {
    }


    void FT_ToolTransform::dy()
    {
    }


    void FT_ToolTransform::dz()
    {
    }


    void FT_ToolTransform::ax()
    {
    }


    void FT_ToolTransform::ay()
    {
    }


    void FT_ToolTransform::az()
    {
    }


    std::string FT_ToolTransform::position_units()
    {
    }


    std::string FT_ToolTransform::rotation_units()
    {
    }



}

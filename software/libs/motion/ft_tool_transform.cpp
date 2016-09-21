#include "ft_tool_transform.hpp"

namespace motion
{

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
        dx_ = value;
    }


    void FT_ToolTransform::set_dy(double value)
    {
        dy_ = value;
    }


    void FT_ToolTransform::set_dz(double value)
    {
        dz_ = value;
    }


    void FT_ToolTransform::set_ax(double value)
    {
        ax_ = value;
    }


    void FT_ToolTransform::set_ay(double value)
    {
        ay_ = value;
    }


    void FT_ToolTransform::set_az(double value)
    {
        az_ = value;
    }


    void FT_ToolTransform::set_position_units(std::string units)
    {
        position_units_ = units;
    }


    void FT_ToolTransform::set_rotation_units(std::string units)
    {
        rotation_units_ = units;
    }


    double FT_ToolTransform::dx()
    {
        return dx_;
    }


    double FT_ToolTransform::dy()
    {
        return dy_;
    }


    double FT_ToolTransform::dz()
    {
        return dz_;
    }


    double FT_ToolTransform::ax()
    {
        return ax_;
    }


    double FT_ToolTransform::ay()
    {
        return ay_;
    }


    double FT_ToolTransform::az()
    {
        return az_;
    }


    std::string FT_ToolTransform::position_units()
    {
        return position_units_;
    }


    std::string FT_ToolTransform::rotation_units()
    {
        return rotation_units_;
    }

    std::vector<char> FT_ToolTransform::position_units_vec()
    {
        std::vector<char> units_vec(position_units_.length()+1,'\0');
        std::copy(position_units_.begin(), position_units_.end(), units_vec.begin());
        return units_vec;
    }

    std::vector<char> FT_ToolTransform::rotation_units_vec()
    {
        std::vector<char> units_vec(rotation_units_.length()+1,'\0');
        std::copy(rotation_units_.begin(), rotation_units_.end(), units_vec.begin());
        return units_vec;
    }

} // namespace motion

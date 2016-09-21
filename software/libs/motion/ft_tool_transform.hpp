#ifndef FT_TOOL_TRANSFORM_HPP
#define FT_TOOL_TRANSFORM_HPP
#include <vector>

namespace motion
{
    class FT_ToolTransform
    {
        public:

            static const std::string DefaultPositionUnits;
            static const std::string DefaultRotationUnits;

            FT_ToolTransform(
                    double dx = 0.0, 
                    double dy = 0.0, 
                    double dz = 0.0, 
                    double ax = 0.0, 
                    double ay = 0.0, 
                    double az = 0.0,
                    std::string position_units = DefaultPositionUnits,
                    std::string rotation_units = DefaultRotationUnits
                    );

            template<typename T> 
            std::vector<T> as_vector();

            void set_dx(double value);
            void set_dy(double value);
            void set_dz(double value);
            void set_ax(double value);
            void set_ay(double value);
            void set_az(double value);
            void set_position_units(std::string units);
            void set_rotation_units(std::string units);

            void dx();
            void dy();
            void dz();
            void ax();
            void ay();
            void az();
            std::string position_units();
            std::string rotation_units();

        protected:

            double dx_;
            double dy_;
            double dz_;
            double ax_;
            double ay_;
            double az_;

            std::string position_units_;
            std::string rotation_units_;

    };

    template<typename T> 
    std::vector<T> FT_ToolTransform::as_vector()
    {
        std::vector<T> vec;
        return vec;
    }
}

#endif

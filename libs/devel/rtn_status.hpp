#ifndef RTN_STATUS_HPP
#define RTN_STATUS_HPP
#include <string>

namespace motion
{
    class RtnStatus
    {
        public:
            RtnStatus(bool success_=true, std::string error_msg=std::string(""));
            bool success();
            void set_success(bool value);

            std::string error_msg();
            void set_error_msg(std::string msg);

        private:
            bool success_;
            std::string error_msg_;
    };

}

#endif

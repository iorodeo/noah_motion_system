#ifndef RTN_STATUS_HPP
#define RTN_STATUS_HPP
#include <string>

namespace mctl
{
    class RtnStatus
    {
        public:
            RtnStatus(bool success_=true, std::string error_msg=std::string(""));

            bool success();
            void set_success(bool value);

            bool user_quit();
            void set_user_quit(bool value);

            std::string error_msg();
            void set_error_msg(std::string msg);

        private:
            bool success_ = true;
            bool user_quit_ = false;
            std::string error_msg_ = std::string("");
    };

}

#endif

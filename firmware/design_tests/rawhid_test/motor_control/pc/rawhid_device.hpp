#ifndef RAWHID_DEVICE_HPP
#define RAWHID_DEVICE_HPP
#include <vector>

extern "C"
{
    #include "hid.h"
}

class RawHIDDevice
{
    public:
        static const int DataBufSize = 64;

        RawHIDDevice(int vid=0, int pid=0, int usage_page=0xFFAB, int usage=0x0200);
        bool open();
        void close();
        int timeout();
        void setTimeout(int timout);
        bool sendData(void *buf);
        bool sendData(std::vector<char> buf);
        bool recvData(void *buf);
        bool recvData(std::vector<char> &buf); 


    protected:
        int vid_ = 0;
        int pid_ = 0;
        int usage_page_ = 0;
        int usage_ = 0;
        int device_num_ = 0; 
        int timeout_ = 100;

};

#endif

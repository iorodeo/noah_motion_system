/*
Copyright 2016 IO Rodeo Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
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
        static const int ClearRecvBufCnt = 10;

        RawHIDDevice(int vid=0, int pid=0, int usage_page=0xFFAB, int usage=0x0200);
        bool open();
        void close();
        int sendTimeout();
        int recvTimeout();
        void setSendTimeout(int timeout);
        void setRecvTimeout(int timeout);
        void clearRecvBuffer();
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
        int recv_timeout_ = 100;
        int send_timeout_ = 100;

};

#endif

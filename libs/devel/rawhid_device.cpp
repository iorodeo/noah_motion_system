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
#include "rawhid_device.hpp"
#include <iostream>
#include <chrono>

RawHIDDevice::RawHIDDevice(int vid, int pid, int usage_page, int usage)
{
    vid_ = vid;
    pid_ = pid;
    usage_page_ = usage_page;
    usage_ = usage;
}


bool RawHIDDevice::open()
{
    bool rval = true;
    int num = rawhid_open(1, vid_, pid_, usage_page_, usage_);
    std::cout << "rawhid num: " << num << std::endl;
    if (num > 0)
    {
        for (int i=1; i<num; i++)
        {
            if (i!=device_num_)
            {
                rawhid_close(i);
            }
        }
    }
    else
    {
        rval = false;
    }
    return rval;
}


void RawHIDDevice::close()
{
    rawhid_close(device_num_);
}


int RawHIDDevice::sendTimeout()
{
    return send_timeout_;
}

int RawHIDDevice::recvTimeout()
{
    return recv_timeout_;
}


void RawHIDDevice::setSendTimeout(int timeout)
{
    if (timeout > 0)
    {
        send_timeout_ = timeout;
    }
    else
    {
        send_timeout_ = 0;
    }
}

void RawHIDDevice::setRecvTimeout(int timeout)
{
    if (timeout > 0)
    {
        recv_timeout_ = timeout;
    }
    else
    {
        recv_timeout_ = 0;
    }
}

void RawHIDDevice::clearRecvBuffer()
{
    char tmp_buf[DataBufSize];
    int recv_timeout_tmp = recv_timeout_;
    recv_timeout_ = 1;
    for (int i=0; i<ClearRecvBufCnt; i++)
    {
        recvData(&tmp_buf);
    };
    recv_timeout_ = recv_timeout_tmp;
}

bool RawHIDDevice::sendData(void *buf)
{
    bool rval = true;
    int num_bytes = rawhid_send(device_num_, buf, DataBufSize, send_timeout_);
    if (num_bytes != DataBufSize)
    {
        rval = false;
    }
    return rval;
}


bool RawHIDDevice::sendData(std::vector<char> buf)
{
    bool rval = true;
    if (buf.size() != DataBufSize)
    {
        rval = false;
    }
    else
    {
        int num_bytes = rawhid_send(device_num_, &buf[0], DataBufSize, send_timeout_);
        if (num_bytes != DataBufSize)
        {
            rval = false;
        }
    }
    return rval;
}


bool RawHIDDevice::recvData(void *buf)
{
    bool rval = true;
    int num_bytes = rawhid_recv(device_num_, buf, DataBufSize, recv_timeout_);
    if (num_bytes != DataBufSize)
    {
        rval = false;
    }
    return rval;
}


bool RawHIDDevice::recvData(std::vector<char> &buf)
{
    bool rval = true;
    if (buf.size() != DataBufSize)
    {
        buf.resize(DataBufSize);
    }
    int num_bytes = rawhid_recv(device_num_, &buf[0], DataBufSize, recv_timeout_);
    if (num_bytes != DataBufSize)
    {
        rval = false;
    }
    return rval;
}

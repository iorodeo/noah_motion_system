#include <string>
#include <sstream>
#include <iostream>
#include <thread>
#include <chrono>
#include <map>

#include "sio_client.h"

int main(int argc, char *argv[])
{
    const int number_of_sends = 2000;
    const int number_of_items = 50;
    const int sleep_dt_ms = 50;

    std::string server_address("http://localhost:3000");
    sio::client sioClient;
    sioClient.connect(server_address);

    // Wait a bit and test that socket is open. Not good, but ok for demo.
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    if (!sioClient.opened())
    {
        std::cout << "couldn't open socket: " << server_address << std::endl;
        exit(1);
    }
    
    // Send data in loop. On each iteration an object with number_of_items is sent to the server.
    for (int i=0; i<number_of_sends; i++) 
    {
        // Create data item to send
        auto msg_ptr = sio::object_message::create();
        std::map<std::string, sio::message::ptr> &user_map = msg_ptr -> get_map();
        for (int j=0; j<number_of_items; j++)
        {
            std::stringstream ss;
            ss << "key_" << j;
            std::pair<std::string, sio::message::ptr> pair(ss.str(), sio::double_message::create(i + j));
            user_map.insert(pair); 
        }

        // Send data item to sever
        sioClient.socket() -> emit("data", msg_ptr);

        std::cout << "msg # " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_dt_ms));
    }

    return 0;
}

#include <iostream>
#include <unistd.h>
#include <boost/asio.hpp>
#include "server.h"

using namespace std;

struct globalArgs_t {
    string ip = "127.0.0.1";
    string dir = ".";
    int port = 12345;
    bool daemon_mode = false;
};

int main(int argc, char** argv)
{
    const char* optString = "mh:p:d:";
    globalArgs_t globalArgs;
    int opt = getopt(argc, argv, optString);
    while (opt != -1)
    {
        switch(opt)
        {
        case 'h':
            globalArgs.ip = optarg;
            break;
        case 'p':
            globalArgs.port = stoi(optarg);
            break;
        case 'd':
            globalArgs.dir = optarg;
            break;
        case 'm':
            globalArgs.daemon_mode = true;
            break;
        default:
            break;
        }
        opt = getopt(argc, argv, optString);
    }

    if(globalArgs.daemon_mode)
        daemon(0, 0);
    chdir(globalArgs.dir.c_str());

    boost::asio::io_service io_service;
    Server server(io_service, globalArgs.ip, globalArgs.port);
    io_service.run();
    return 0;
}

#ifndef LOGIN_SERVER_HEADER
#define LOGIN_SERVER_HEADER

#include <iostream>
#include <arpa/inet.h>

using namespace std;

class HachiServer
{
public:
    HachiServer();
    void run();

private:
    void dispatch_message();
    void dispatch_send(const char* message);
    void process_message(const char* message);

    int _dispatch_server_socket;
    struct sockaddr_in _dispatch_server;
    char buffer[64];
};

#endif

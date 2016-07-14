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
    void on_message();
    void on_send(const char* message);

    int _dispatch_socket;
    struct sockaddr_in _dispatch_server;
    char buffer[64];
};

#endif

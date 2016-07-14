#ifndef LOGIN_SERVER_HEADER
#define LOGIN_SERVER_HEADER

#include <iostream>
#include <arpa/inet.h>
#include <netdb.h>

using namespace std;

class HachiServer
{
public:
    HachiServer();
    void run();
    void send();
};

#endif
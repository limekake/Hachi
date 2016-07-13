#ifndef NETWORKING
#define NETWORKING
#define THREADS 4
#define DISPATCH_SERVER_PORT 8081
#define LOGIN_SERVER_PORT 8082
#define MAP_SERVER_PORT 8082

#include <uWS/uWS.h>

class HachiNetwork
{
public:
    explicit HachiNetwork(int port) : _server(port) {}
    virtual ~HachiNetwork() {}

    virtual void run() = 0;
    virtual void on_connect(uWS::WebSocket socket) = 0;
    virtual void on_disconnect(uWS::WebSocket socket) = 0;
    virtual void on_message(uWS::WebSocket socket, char *message, size_t length, uWS::OpCode opCode) = 0;

protected:
    uWS::Server _server;
};

enum SERVER_TYPE
{
    DISPATCH = 1,
    LOGIN,
    MAP,
};

#endif
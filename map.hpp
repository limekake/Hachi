#ifndef MAP_SERVER_HEADER
#define MAP_SERVER_HEADER

#include <uWS/uWS.h>
#include "network.hpp"

using namespace std;

class HachiServer : public HachiNetwork
{
public:
    HachiServer();
    void on_connect(uWS::WebSocket socket) override;
    void on_disconnect(uWS::WebSocket socket) override;
    void on_message(uWS::WebSocket socket, char *message, size_t length, uWS::OpCode opCode) override;

private:
    static void process_message(const char *message);

    uWS::WebSocket _dispatch_server;
};

#endif
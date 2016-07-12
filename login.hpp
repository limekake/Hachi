#ifndef LOGIN_SERVER
#define LOGIN_SERVER

#include "network.hpp"
#include <uWS/uWS.h>

using namespace uWS;
using namespace std;

class HachiServer : public HachiNetwork
{
public:
    HachiServer();
    void on_connect(WebSocket socket) override;
    void on_disconnect(WebSocket socket) override;
    void on_message(WebSocket socket, char *message, size_t length, OpCode opCode) override;
};

enum ACTION
{
    GET_USER = 1,
    CREATE_USER,
    DELETE_USER,
    UPDATE_USER,
    MESSAGE_CHAT,
};

#endif
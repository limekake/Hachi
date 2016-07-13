#ifndef DISPATCH_SERVER_HEADER
#define DISPATCH_SERVER_HEADER

#include <iostream>
#include <uWS/uWS.h>
#include "easywsclient/easywsclient.hpp"
#include "network.hpp"
#include <map>

using namespace std;

struct connection_session {
    uWS::WebSocket socket;
    bool auth = false;
};

class HachiServer : public HachiNetwork
{
public:
    HachiServer();
    void run() override;
    void on_connect(uWS::WebSocket socket) override;
    void on_disconnect(uWS::WebSocket socket) override;
    void on_message(uWS::WebSocket socket, char *message, size_t length, uWS::OpCode opCode) override;

private:
    connection_session* get_session(uWS::WebSocket socket)
    {
        auto session = _connection_pool.find(socket);

        if (session == _connection_pool.end())
        {
            cout << "No session found!" << endl;
            throw;
        }
        return &(session->second);
    }

    easywsclient::WebSocket::pointer _login_server_ws;
    easywsclient::WebSocket::pointer _map_server_ws;
    int _next_sessionid;
    map<uWS::WebSocket, connection_session> _connection_pool;
};

#endif
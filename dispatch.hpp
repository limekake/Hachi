#ifndef DISPATCH_SERVER_HEADER
#define DISPATCH_SERVER_HEADER

#include <iostream>
#include <arpa/inet.h>
#include <uWS/uWS.h>
#include <map>

using namespace std;

struct connection_session {
    uWS::WebSocket socket;
    int session_id;
    bool auth = false;
};

class HachiServer
{
public:
    HachiServer();
    void run();
    void on_connect(uWS::WebSocket socket) ;
    void on_disconnect(uWS::WebSocket socket) ;
    void on_message(uWS::WebSocket socket, char *message, size_t length, uWS::OpCode opCode);

private:
    void login_message();
    void login_send(const char* message, size_t size);
    void login_process_message(const char* message);

    uWS::Server _outside_server;
    int _next_sessionid;
    map<uWS::WebSocket, connection_session> _connection_pool;

    int _dispatch_server_socket;
    int _login_server_socket;
    struct sockaddr_in _dispatch_server;
    struct sockaddr_in _login_server;

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

    connection_session* get_session(int session_id)
    {
        for (auto it = _connection_pool.begin(); it != _connection_pool.end(); ++it)
        {
            if (it->second.session_id == session_id)
            {
                return &(it->second);
            }
        }
        return nullptr;
    }
};

#endif

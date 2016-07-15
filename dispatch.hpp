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

    string username;
    bool auth = false;
};

class HachiServer
{
public:
    HachiServer();
    void run();

private:
    void on_connect(uWS::WebSocket socket) ;
    void on_disconnect(uWS::WebSocket socket) ;
    void on_message(uWS::WebSocket socket, char *message, size_t length, uWS::OpCode opCode);

    void internal_server_send(int server_socket, const char* message, size_t size);
    void generic_server_listen();
    void generic_server_handler(int socket);
    void login_server_handler();
    void login_process_message(const char* message);
    void map_server_handler();

    uWS::Server _outside_server;
    map<uWS::WebSocket, int> _connection_pool;
    map<int, connection_session> _session_pool;

    struct sockaddr_in _dispatch_server;
    int _next_session_id;
    int _dispatch_server_socket;
    int _login_server_socket;
    int _map_server_socket;

    connection_session* get_session(uWS::WebSocket socket)
    {
        return get_connection_session(get_session_id(socket));
    }

    int get_session_id(uWS::WebSocket socket)
    {
        auto pair = _connection_pool.find(socket);
        if (pair == _connection_pool.end())
        {
            cout << "No session id is mapped to this connection" << endl;
            exit(1);
        }
        return (pair->second);
    }

    connection_session* get_connection_session(int id)
    {
        auto pair = _session_pool.find(id);

        if (pair == _session_pool.end())
        {
            cout << "No session found!" << endl;
            throw;
        }
        return &(pair->second);
    }
};

#endif

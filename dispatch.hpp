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
    map<int, connection_session> _connection_pool;

    struct sockaddr_in _dispatch_server;
    int _dispatch_server_socket;
    int _login_server_socket;
    int _map_server_socket;

    connection_session* get_session(int id)
    {
        auto session = _connection_pool.find(id);

        if (session == _connection_pool.end())
        {
            cout << "No session found!" << endl;
            throw;
        }
        return &(session->second);
    }
};

#endif

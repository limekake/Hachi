#include <iostream>
#include <cstring>
#include <thread>
#include <netdb.h>
#include <arpa/inet.h>
#include <uWS/uWS.h>
#include "dispatch.hpp"
#include "network.hpp"
#include "packet.hpp"

using namespace std;

HachiServer::HachiServer() : _outside_server(OUTSIDE_SERVER_PORT), _next_session_id(1)
{
    _outside_server.onConnection(bind(&HachiServer::on_connect, this, placeholders::_1));
    _outside_server.onDisconnection(bind(&HachiServer::on_disconnect, this, placeholders::_1));
    _outside_server.onMessage(bind(&HachiServer::on_message, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4));

    _dispatch_server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_dispatch_server_socket < 0)
    {
        cout << "[DISPATCH] Error creating socket" << endl;
        exit(1);
    }
    _dispatch_server.sin_family = AF_INET;
    _dispatch_server.sin_addr.s_addr = inet_addr("127.0.0.1");
    _dispatch_server.sin_port = htons(DISPATCH_SERVER_PORT);
}

void HachiServer::run()
{
    if (bind(_dispatch_server_socket, (struct sockaddr *) &_dispatch_server, sizeof(_dispatch_server)) < 0) {
        cout << "[DISPATCH] Error binding on port " << DISPATCH_SERVER_PORT << endl;
        exit(1);
    }
    listen(_dispatch_server_socket, 2);
    cout << "[DISPATCH] Internal listening on port " << DISPATCH_SERVER_PORT << endl;

    thread _generic_server_thread(&HachiServer::generic_server_listen, this);

    cout << "[DISPATCH] External listening on port " << OUTSIDE_SERVER_PORT << endl;
    _outside_server.run();
    _generic_server_thread.join();
}

void HachiServer::generic_server_listen()
{
    struct sockaddr_in _generic_server;
    int c = sizeof(_generic_server);
    while (true)
    {
        int generic_socket = accept(_dispatch_server_socket, (struct sockaddr *) &_generic_server, (socklen_t *)&c);
        thread (&HachiServer::generic_server_handler, this, generic_socket).detach();
    }
}

void HachiServer::generic_server_handler(int socket)
{
    char buffer[64];
    recv(socket, buffer, 64, 0);

    SERVER_CONNECT server_connect;
    memcpy(&server_connect, buffer, sizeof(SERVER_CONNECT));
    switch (server_connect.server_type)
    {
        case SERVER_TYPE::LOGIN:
        {
            _login_server_socket = socket;
            cout << "[DISPATCH] Login server has connected" << endl;
            thread (&HachiServer::login_server_handler, this).detach();
            break;
        }
        case SERVER_TYPE::MAP:
        {
            _map_server_socket = socket;
            cout << "[DISPATCH] Map server has connected" << endl;
            thread (&HachiServer::map_server_handler, this).detach();
            break;
        }
    }
}

void HachiServer::login_server_handler()
{
    int recv_size;
    char buffer[64];

    while ((recv_size = recv(_login_server_socket, buffer, 64, 0)) > 0)
    {
        login_process_message(buffer);
        memset(buffer, 0, 64);
    }
}

void HachiServer::login_process_message(const char* message)
{
    RESPONSE_LOGIN_INTERNAL response_login_internal;
    memcpy(&response_login_internal, message, sizeof(RESPONSE_LOGIN_INTERNAL));

    auto session = get_connection_session(response_login_internal.session_id);
    session->auth = true;

    cout << "[DISPATCH] Login authenticated session " << session->session_id <<  endl;

    RESPONSE_LOGIN_EXTERNAL response_login_external;
    auto response_message = new char[sizeof(RESPONSE_LOGIN_EXTERNAL)];
    response_login_external.auth = true;
    memcpy(static_cast<void*>(response_message), static_cast<void*>(&response_login_external), sizeof(RESPONSE_LOGIN_EXTERNAL));
    session->socket.send(response_message, sizeof(response_message), uWS::OpCode::BINARY);
}

void HachiServer::map_server_handler()
{
}

void HachiServer::internal_server_send(int server_socket, const char* message, size_t size)
{
    send(server_socket, message, size, 0);
}

void HachiServer::on_connect(uWS::WebSocket socket)
{
    connection_session new_session;
    new_session.socket = socket;
    new_session.session_id = _next_session_id++;

    _connection_pool[new_session.socket] = new_session.session_id;
    _session_pool[new_session.session_id] = new_session;

    cout << "[DISPATCH] Connect: " << socket.getAddress().address << ":" << socket.getAddress().port << endl;
}

void HachiServer::on_disconnect(uWS::WebSocket socket)
{
    auto id = get_session_id(socket);

    cout << "[DISPATCH] Disconnect: " << socket.getAddress().address << ":" << socket.getAddress().port << endl;

    _session_pool.erase(id);
    _connection_pool.erase(socket);
}

void HachiServer::on_message(uWS::WebSocket socket, char *message, size_t length, uWS::OpCode opCode)
{
    auto session = get_session(socket);
    auto message_data = string(message, length).c_str();

    if (!session->auth)
    {
        //REQUEST_LOGIN_EXTERNAL request_login_external;
        //request_login_external.username = string(message_data);
        //memcpy(&request_login_external, message_data, sizeof(REQUEST_LOGIN_EXTERNAL));

        //cout << request_login_external.username << endl;

        auto pass_message = new char[sizeof(REQUEST_LOGIN_INTERNAL)];
        REQUEST_LOGIN_INTERNAL request_login_internal;
        request_login_internal.session_id = session->session_id;
        strncpy(request_login_internal.username, message_data, 20);
        memcpy(static_cast<void*>(pass_message), static_cast<void*>(&request_login_internal), sizeof(REQUEST_LOGIN_INTERNAL));

        internal_server_send(_login_server_socket, pass_message, sizeof(REQUEST_LOGIN_INTERNAL));
    }
    else
    {
        cout << "[DISPATCH] Message: " << message_data << endl;
    }
}

int main(int argc, char *argv[])
{
    HachiServer _server;
    _server.run();

    return 0;
}

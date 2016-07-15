#include <iostream>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <netdb.h>
#include <thread>
#include "login.hpp"
#include "packet.hpp"
#include "network.hpp"

using namespace std;

HachiServer::HachiServer()
{
    _dispatch_server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_dispatch_server_socket < 0)
    {
        cout << "[LOGIN] Error creating socket" << endl;
        exit(1);
    }

    _dispatch_server.sin_addr.s_addr = inet_addr("127.0.0.1");
    _dispatch_server.sin_family = AF_INET;
    _dispatch_server.sin_port = htons(DISPATCH_SERVER_PORT);
}

void HachiServer::run()
{
    if (connect(_dispatch_server_socket, (struct sockaddr *)&_dispatch_server, sizeof(_dispatch_server)) < 0)
    {
        cout << "[LOGIN] Error connecting to dispatch" << endl;
        exit(1);
    }

    auto server_connect_message = new char[sizeof(SERVER_CONNECT)];
    SERVER_CONNECT server_connect;
    server_connect.server_type = SERVER_TYPE::LOGIN;
    memcpy(static_cast<void*>(server_connect_message), static_cast<void*>(&server_connect), sizeof(SERVER_CONNECT));

    send(_dispatch_server_socket, server_connect_message, sizeof(SERVER_CONNECT), 0);

    cout << "[LOGIN] Connected to dispatch" << endl;

    thread _dispatch_thread(&HachiServer::dispatch_server_handler, this);

    _dispatch_thread.join();
}

void HachiServer::dispatch_server_handler()
{
    int recv_size;
    char buffer[64];

    while ((recv_size = recv(_dispatch_server_socket, buffer, 64, 0)) > 0)
    {
        process_message(buffer);
        memset(buffer, 0, 64);
    }
}

void HachiServer::dispatch_send(const char* message, size_t size)
{
    send(_dispatch_server_socket, message, size, 0);
}

void HachiServer::process_message(const char *message)
{
    REQUEST_LOGIN_INTERNAL request_login_internal;
    memcpy(&request_login_internal, message, sizeof(REQUEST_LOGIN_INTERNAL));

    cout << "[LOGIN] User: " << request_login_internal.username <<  " " << request_login_internal.session_id << endl;

    RESPONSE_LOGIN_INTERNAL response_login_internal;
    auto response_message = new char[sizeof(RESPONSE_LOGIN_INTERNAL)];
    response_login_internal.session_id = request_login_internal.session_id;
    memcpy(static_cast<void*>(response_message), static_cast<void*>(&response_login_internal), sizeof(RESPONSE_LOGIN_INTERNAL));

    dispatch_send(response_message, sizeof(RESPONSE_LOGIN_INTERNAL));
}

int main(int argc, char *argv[])
{
    HachiServer _server;
    _server.run();

    return 0;
}

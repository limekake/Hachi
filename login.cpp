#include <iostream>
#include <string>
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
    cout << "[LOGIN] Connected to dispatch" << endl;

    thread _dispatch_thread(&HachiServer::dispatch_message, this);

    _dispatch_thread.join();
}

void HachiServer::dispatch_message()
{
    int recv_size;
    char buffer[64];
    string message;

    while ((recv_size = recv(_dispatch_server_socket, buffer, 64, 0)) > 0)
    {
        process_message(buffer);
        memset(buffer, 0, 64);
    }
}

void HachiServer::dispatch_send(const char* message)
{
    send(_dispatch_server_socket, message, sizeof(message), 0);
}

void HachiServer::process_message(const char *message)
{
    REQUEST_LOGIN login_request;
    memcpy(&login_request, message, sizeof(REQUEST_LOGIN));

    cout << "[LOGIN] User: " << login_request.username <<  " " << login_request.session_id << endl;

    /*RESPONSE_LOGIN login_response;
    auto response_message = new char[sizeof(RESPONSE_LOGIN)];
    login_response.session_id = login_request.session_id;
    memcpy(static_cast<void*>(&login_response), static_cast<void*>(response_message), sizeof(RESPONSE_LOGIN));

    dispatch_send(response_message);*/
}
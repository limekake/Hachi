#include <iostream>
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
    _dispatch_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_dispatch_socket < 0)
    {
        cout << "[LOGIN] Error creating socket" << endl;
        exit(1);
    }

    _dispatch_server.sin_addr.s_addr = inet_addr("127.0.0.1");
    _dispatch_server.sin_family = AF_INET;
    _dispatch_server.sin_family = htons(DISPATCH_SERVER_PORT);
}

void HachiServer::run()
{
    if (connect(_dispatch_socket, (struct sockaddr *)&_dispatch_server, sizeof(_dispatch_server)) < 0)
    {
        cout << "[LOGIN] Error connecting to dispatch" << endl;
        exit(1);
    }
    cout << "[LOGIN] Connected to dispatch" << endl;

    thread _dispatch_thread(&HachiServer::on_message, this);
}

void HachiServer::on_message()
{
    char buffer[64];
    string message;

    while (true)
    {
        recv(_dispatch_socket, buffer, sizeof(buffer), 0);
        cout << buffer << endl;
    }
}

void HachiServer::on_send(const char* message)
{
    send(_dispatch_socket, message, sizeof(message), 0);
}

//void HachiServer::process_message(const char *message)
//{
//    REQUEST_LOGIN login_request;
//    memcpy(&login_request, message, sizeof(REQUEST_LOGIN));
//
//    //cout << "[LOGIN] User: " << string(login_request.username) <<  " " << login_request.session_id << endl;
//
//    RESPONSE_LOGIN login_response;
//    login_response.session_id = login_request.session_id;
//    auto response_message = new char[sizeof(RESPONSE_LOGIN)];
//    memcpy(static_cast<void*>(&login_response), static_cast<void*>(response_message), sizeof(RESPONSE_LOGIN));
//}
